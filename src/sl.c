#include "sl.h"

#include "internal/shaders.h"
#include "internal/triangle.h"
#include "internal/rectangle.h"
#include "internal/circle.h"
#include "internal/point.h"
#include "internal/line.h"
#include "internal/sprite.h"
#include "internal/text.h"

#include "util/gldebugging.h"
#include "util/transform.h"
#include "util/images.h"

#include <gl/glew.h>

#include <glfw/glfw3.h>

#include <windows.h>
#include <stdlib.h>
#include <stdio.h>

#define SL_MATRIX_STACK_SIZE 32

#define IDEAL_FRAME_TIME 0.01666667

// private vars

static GLFWwindow *slProgramWindow = NULL;
static int slWindowWidth = 0;
static int slWindowHeight = 0;

static Mat4 slMatrixStack[SL_MATRIX_STACK_SIZE];
static Mat4 *slCurrentMatrix = &slMatrixStack[0];
static int slStackSize = 0;

static Mat4 slProjectionMatrix;

static Vec4 slForeColor = {.x = 1.0, .y = 1.0, .z = 1.0, .w = 1.0};

static float slSpriteScrollX = 0.0;
static float slSpriteScrollY = 0.0;
static float slSpriteTilingX = 1.0;
static float slSpriteTilingY = 1.0;

static int slTextAlign = SL_ALIGN_LEFT;

static float slDeltaTime = IDEAL_FRAME_TIME;
static float slOldFrameTime = 0.0;
static float slNewFrameTime = IDEAL_FRAME_TIME;

// private function prototypes

static void slInitResources();
static void slKillResources();

// window commands

void slWindow(int width, int height, const char *title)
{
	// types enabling us to access WGL functionality for enabling vsync in Windows
    typedef BOOL (WINAPI *PFNWGLSWAPINTERVALEXTPROC)(int interval);
    PFNWGLSWAPINTERVALEXTPROC wglSwapIntervalEXT = NULL;

	// error tracking for any window-creation issues we run into
	GLenum error;

	if(slProgramWindow == NULL)
	{
		// start up GLFW
		glfwInit();

		// set our OpenGL context to something that doesn't support any old-school shit
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_REFRESH_RATE, 60);

		// enable OpenGL debugging context if we're in a debug build
		#ifdef DEBUG
			glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
		#endif

		// create our OpenGL window
		slProgramWindow = glfwCreateWindow(width, height, title, NULL, NULL);
		glfwMakeContextCurrent(slProgramWindow);
		glfwSwapInterval(1);

		// GLFW doesn't handle vsync well in all cases, so we have to go straight to WGL to do this
		wglSwapIntervalEXT = (PFNWGLSWAPINTERVALEXTPROC)wglGetProcAddress("wglSwapIntervalEXT");
		wglSwapIntervalEXT(1);

		// configure our viewing area
		glViewport(0, 0, width, height);

		// enable our extensions handler
		glewExperimental = 1;
		error = glewInit();
		if(error != GLEW_OK)
		{
			fprintf(stderr, "slWindow() could not initialize GLEW: %s\n", glewGetErrorString(error));
			exit(1);
		}

		// clear the OpenGL error resulting from glewInit()
		glGetError();

		// turn on OpenGL debugging
		#ifdef DEBUG
			initGLDebugger();
		#endif

		// turn on blending and turn depth testing off
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glDisable(GL_DEPTH_TEST);

		// camera view settings
		slProjectionMatrix = ortho(0.0f, (float)width, 0.0f, (float)height);
		slWindowWidth = width;
		slWindowHeight = height;

		// default colors
		slSetBackColor(0.0, 0.0, 0.0);
		slSetForeColor(1.0, 1.0, 1.0, 1.0);

		// initialize any rendering resources
		slInitResources();

		// initialize our first transformation matrix
		*slCurrentMatrix = identity();
	}
	else
	{
		fprintf(stderr, "slWindow() cannot be called because a window already exists\n");
		exit(1);
	}
}

void slClose()
{
	if(slProgramWindow != NULL)
	{
		slKillResources();

		glfwDestroyWindow(slProgramWindow);
		glfwTerminate();
		slProgramWindow = NULL;
	}
	else
	{
		fprintf(stderr, "slClose() cannot be called because no window exists\n");
		exit(1);
	}
}

// simple input

int slGetKey(int key)
{
	return glfwGetKey(slProgramWindow, key) == GLFW_PRESS;
}

int slGetMouseButton(int button)
{
	return glfwGetMouseButton(slProgramWindow, button) == GLFW_PRESS;
}

void slGetMousePos(int *posX, int *posY)
{
	double x, y;
	glfwGetCursorPos(slProgramWindow, &x, &y);
	*posX = x;
	*posY = slWindowHeight - y;
}

// simple frame timing

float slGetDeltaTime()
{
	return slDeltaTime;
}

// rendering/clearing commands

void slRender()
{
	// value close enough to zero for delta time management
	const double SL_MIN_DELTA_TIME = 0.00001;			// tiny fraction of a second
	const double SL_MAX_DELTA_TIME = 0.5;				// half a second dt max

	// render any leftover points or lines
	sliPointsFlush();
	sliLinesFlush();
	sliTextFlush(slCurrentMatrix, &slForeColor);

	// read any input events, show the back buffer, and clear the (previous) front buffer
	glfwPollEvents();
	glfwSwapBuffers(slProgramWindow);
	glClear(GL_COLOR_BUFFER_BIT);

	// gather time values
	slOldFrameTime = slNewFrameTime;
	slNewFrameTime = glfwGetTime();

	// compute delta time value; ensure we don't have any long pauses or tiny time quantums
	slDeltaTime = (slNewFrameTime - slOldFrameTime);
	if(slDeltaTime < SL_MIN_DELTA_TIME)
		slDeltaTime = SL_MIN_DELTA_TIME;
	if(slDeltaTime > SL_MAX_DELTA_TIME)
		slDeltaTime = SL_MAX_DELTA_TIME;
}

// colour control

void slSetBackColor(float red, float green, float blue)
{
	glClearColor(red, green, blue, 1.0);
}

void slSetForeColor(float red, float green, float blue, float alpha)
{
	slForeColor.x = red;
	slForeColor.y = green;
	slForeColor.z = blue;
	slForeColor.w = alpha;
}

// blending control

void slSetAdditiveBlend(int additiveBlend)
{
	sliPointsFlush();
	sliLinesFlush();

	glBlendFunc(GL_SRC_ALPHA, additiveBlend ? GL_ONE : GL_ONE_MINUS_SRC_ALPHA);
}

// transformations

void slPush()
{
	if(slStackSize < SL_MATRIX_STACK_SIZE)
	{
		slStackSize ++;
		slCurrentMatrix ++;
		*slCurrentMatrix = *(slCurrentMatrix - 1);
	}
	else
	{
		fprintf(stderr, "slPush() exceeded maximum transform stack size of %d\n", SL_MATRIX_STACK_SIZE);
		exit(1);
	}
}

void slPop()
{
	if(slStackSize > 0)
	{
		slStackSize --;
		slCurrentMatrix --;
	}
	else
	{
		fprintf(stderr, "slPop() cannot pop an empty transform stack\n");
		exit(1);
	}
}

void slTranslate(float x, float y)
{
	*slCurrentMatrix = translate(slCurrentMatrix, x, y);
}

void slRotate(float degrees)
{
	*slCurrentMatrix = rotate(slCurrentMatrix, (float)degrees);
}

void slScale(float x, float y)
{
	*slCurrentMatrix = scale(slCurrentMatrix, x, y);
}

// texture loading

int slLoadTexture(const char *filename)
{
	return (int)loadOpenGLTexture(filename);
}

// simple shape commands

void slTriangleFill(float x, float y, float width, float height)
{
	Mat4 modelview = translate(slCurrentMatrix, x, y);
	modelview = scale(&modelview, width, height);

	sliPointsFlush();
	sliLinesFlush();
	sliTextFlush(slCurrentMatrix, &slForeColor);
	sliTriangleFill(&modelview, &slForeColor);
}

void slTriangleOutline(float x, float y, float width, float height)
{
	Mat4 modelview = translate(slCurrentMatrix, x, y);
	modelview = scale(&modelview, width, height);

	sliPointsFlush();
	sliLinesFlush();
	sliTextFlush(slCurrentMatrix, &slForeColor);
	sliTriangleOutline(&modelview, &slForeColor);
}

void slRectangleFill(float x, float y, float width, float height)
{
	Mat4 modelview = translate(slCurrentMatrix, x, y);
	modelview = scale(&modelview, width, height);

	sliPointsFlush();
	sliLinesFlush();
	sliTextFlush(slCurrentMatrix, &slForeColor);
	sliRectangleFill(&modelview, &slForeColor);
}

void slRectangleOutline(float x, float y, float width, float height)
{
	Mat4 modelview = translate(slCurrentMatrix, x, y);
	modelview = scale(&modelview, width, height);

	sliPointsFlush();
	sliLinesFlush();
	sliTextFlush(slCurrentMatrix, &slForeColor);
	sliRectangleOutline(&modelview, &slForeColor);
}

void slCircleFill(float x, float y, float radius, int numVertices)
{
	Mat4 modelview = translate(slCurrentMatrix, x, y);

	sliPointsFlush();
	sliLinesFlush();
	sliTextFlush(slCurrentMatrix, &slForeColor);
	sliCircleFill(&modelview, &slForeColor, radius, numVertices);
}

void slCircleOutline(float x, float y, float radius, int numVertices)
{
	Mat4 modelview = translate(slCurrentMatrix, x, y);

	sliPointsFlush();
	sliLinesFlush();
	sliTextFlush(slCurrentMatrix, &slForeColor);
	sliCircleOutline(&modelview, &slForeColor, radius, numVertices);
}

void slPoint(float x, float y)
{
	Mat4 modelview = translate(slCurrentMatrix, x, y);

	sliLinesFlush();
	sliTextFlush(slCurrentMatrix, &slForeColor);
	sliPoint(&modelview, &slForeColor);
}

void slLine(float x1, float y1, float x2, float y2)
{
	Mat4 modelview1 = translate(slCurrentMatrix, x1, y1);
	Mat4 modelview2 = translate(slCurrentMatrix, x2, y2);

	sliPointsFlush();
	sliTextFlush(slCurrentMatrix, &slForeColor);
	sliLine(&slForeColor, modelview1.cols[3].x, modelview1.cols[3].y, modelview2.cols[3].x, modelview2.cols[3].y);
}

void slSetSpriteTiling(float x, float y)
{
	slSpriteTilingX = x;
	slSpriteTilingY = y;
}

void slSetSpriteScroll(float x, float y)
{
	slSpriteScrollX = x;
	slSpriteScrollY = y;
}

void slSprite(int texture, float x, float y, float width, float height)
{
	Mat4 modelview;
	Vec2 tiling = {.x = slSpriteTilingX, .y = slSpriteTilingY};
	Vec2 scroll = {.x = slSpriteScrollX, .y = slSpriteScrollY};

	modelview = translate(slCurrentMatrix, x, y);
	modelview = scale(&modelview, width, height);

	sliPointsFlush();
	sliLinesFlush();
	sliTextFlush(slCurrentMatrix, &slForeColor);
	sliSprite(&modelview, &slForeColor, (GLuint)texture, &tiling, &scroll);
}

// text commands

void slSetTextAlign(int fontAlign)
{
	if(fontAlign >= 0 &&fontAlign <= 2)
	{
		slTextAlign = fontAlign;
	}
	else
	{
		fprintf(stderr, "slSetTextAlign() only accepts SL_ALIGN_CENTER, SL_ALIGN_RIGHT, or SL_ALIGN_LEFT\n");
		exit(1);
	}
}

float slGetTextWidth(const char *text)
{
	return sliTextWidth(text);
}

float slGetTextHeight(const char *text)
{
	return sliTextHeight(text);
}

void slSetFont(const char *fontFilename, int fontSize)
{
	sliFont(fontFilename, fontSize);
}

void slSetFontSize(int fontSize)
{
	sliFontSize(fontSize);
}

void slText(float x, float y, const char *text)
{
	Mat4 modelview = translate(slCurrentMatrix, x, y);

	if(slTextAlign == SL_ALIGN_CENTER)
	{
		modelview = translate(&modelview, -slGetTextWidth(text) / 2.0, 0.0);
	}
	else if(slTextAlign == SL_ALIGN_RIGHT)
	{
		modelview = translate(&modelview, -slGetTextWidth(text), 0.0);
	}

	sliPointsFlush();
	sliLinesFlush();
	sliText(&modelview, &slForeColor, text);
}

// private functions

void slInitResources()
{
	sliShadersInit(&slProjectionMatrix);
	sliTriangleInit();
	sliRectangleInit();
	sliCircleInit();
	sliPointInit();
	sliLineInit();
	sliSpriteInit();
	sliTextInit();
}

void slKillResources()
{
	sliTextDestroy();
	sliSpriteDestroy();
	sliLineDestroy();
	sliPointDestroy();
	sliCircleDestroy();
	sliRectangleDestroy();
	sliTriangleDestroy();
	sliShadersDestroy();
}
