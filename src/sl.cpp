#include "sl.h"

#include "internal/shaders.h"
#include "internal/triangle.h"
#include "internal/rectangle.h"
#include "internal/circle.h"
#include "internal/point.h"
#include "internal/line.h"
#include "internal/sprite.h"
#include "internal/text.h"

#include "assets/assetmanager.h"

#include "util/gldebugging.h"

#include <gl/glew.h>

#include <glfw/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

#include <cstdlib>
#include <cstdio>

#define SL_MATRIX_STACK_SIZE 32

// private vars

static GLFWwindow *slProgramWindow = NULL;
static int slWindowWidth = 0;
static int slWindowHeight = 0;

static mat4 slMatrixStack[SL_MATRIX_STACK_SIZE];
static mat4 *slCurrentMatrix = &slMatrixStack[0];

static mat4 slProjectionMatrix;

static vec4 slForeColor = vec4(1.0, 1.0, 1.0, 1.0);

static int slTextAlign = SL_ALIGN_LEFT;

// private function prototypes

static void slInitResources();
static void slKillResources();

// window commands

void slWindow(int width, int height, const char *title)
{
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

		// configure our viewing area
		glViewport(0, 0, width, height);

		// enable our extensions handler
		glewExperimental = true;
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
		*slCurrentMatrix = mat4(1.0);
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

bool slGetKey(int key)
{
	return glfwGetKey(slProgramWindow, key) == GLFW_PRESS;
}

bool slGetMouseButton(int button)
{
	return glfwGetMouseButton(slProgramWindow, button) == GLFW_PRESS;
}

void slGetMousePos(int &posX, int &posY)
{
	double x, y;
	glfwGetCursorPos(slProgramWindow, &x, &y);
	posX = x;
	posY = slWindowHeight - y;
}

// rendering/clearing commands

void slRender()
{
	// render any leftover points or lines
	sliPointsFlush();
	sliLinesFlush();
	sliTextFlush(*slCurrentMatrix, slForeColor);

	// read any input events, show the back buffer, and clear the (previous) front buffer
	glfwPollEvents();
	glfwSwapBuffers(slProgramWindow);
	glClear(GL_COLOR_BUFFER_BIT);
}

// colour control

void slSetBackColor(double red, double green, double blue)
{
	glClearColor(red, green, blue, 1.0);
}

void slSetForeColor(double red, double green, double blue, double alpha)
{
	slForeColor = vec4(red, green, blue, alpha);
}

// blending control

void slSetAdditiveBlend(bool additiveBlend)
{
	sliPointsFlush();
	sliLinesFlush();

	glBlendFunc(GL_SRC_ALPHA, additiveBlend ? GL_ONE : GL_ONE_MINUS_SRC_ALPHA);
}

// transformations

void slPush()
{
	slCurrentMatrix ++;
	*slCurrentMatrix = *(slCurrentMatrix - 1);
}

void slPop()
{
	slCurrentMatrix --;
}

void slTranslate(double x, double y)
{
	*slCurrentMatrix = translate(*slCurrentMatrix, vec3(x, y, 0.0));
}

void slRotate(double degrees)
{
	*slCurrentMatrix = rotate(*slCurrentMatrix, (float)degrees, vec3(0.0, 0.0, 1.0));
}

void slScale(double x, double y)
{
	*slCurrentMatrix = scale(*slCurrentMatrix, vec3(x, y, 1.0));
}

// simple shape commands

void slTriangleFill(double x, double y, double width, double height)
{
	mat4 modelview = translate(*slCurrentMatrix, vec3(x, y, 0.0));
	modelview = scale(*slCurrentMatrix, vec3(width, height, 1.0));

	sliPointsFlush();
	sliLinesFlush();
	sliTextFlush(*slCurrentMatrix, slForeColor);
	sliTriangleFill(modelview, slForeColor);
}

void slTriangleOutline(double x, double y, double width, double height)
{
	mat4 modelview = translate(*slCurrentMatrix, vec3(x, y, 0.0));
	modelview = scale(*slCurrentMatrix, vec3(width, height, 1.0));

	sliPointsFlush();
	sliLinesFlush();
	sliTextFlush(*slCurrentMatrix, slForeColor);
	sliTriangleOutline(modelview, slForeColor);
}

void slRectangleFill(double x, double y, double width, double height)
{
	mat4 modelview = translate(*slCurrentMatrix, vec3(x, y, 0.0));
	modelview = scale(*slCurrentMatrix, vec3(width, height, 1.0));

	sliPointsFlush();
	sliLinesFlush();
	sliTextFlush(*slCurrentMatrix, slForeColor);
	sliRectangleFill(modelview, slForeColor);
}

void slRectangleOutline(double x, double y, double width, double height)
{
	mat4 modelview = translate(*slCurrentMatrix, vec3(x, y, 0.0));
	modelview = scale(*slCurrentMatrix, vec3(width, height, 1.0));

	sliPointsFlush();
	sliLinesFlush();
	sliTextFlush(*slCurrentMatrix, slForeColor);
	sliRectangleOutline(modelview, slForeColor);
}

void slCircleFill(double x, double y, double radius, int numVertices)
{
	mat4 modelview = translate(*slCurrentMatrix, vec3(x, y, 0.0));

	sliPointsFlush();
	sliLinesFlush();
	sliTextFlush(*slCurrentMatrix, slForeColor);
	sliCircleFill(modelview, slForeColor, radius, numVertices);
}

void slCircleOutline(double x, double y, double radius, int numVertices)
{
	mat4 modelview = translate(*slCurrentMatrix, vec3(x, y, 0.0));

	sliPointsFlush();
	sliLinesFlush();
	sliTextFlush(*slCurrentMatrix, slForeColor);
	sliCircleOutline(modelview, slForeColor, radius, numVertices);
}

void slPoint(double x, double y)
{
	mat4 modelview = translate(*slCurrentMatrix, vec3(x, y, 0.0));

	sliLinesFlush();
	sliTextFlush(*slCurrentMatrix, slForeColor);
	sliPoint(modelview, slForeColor);
}

void slLine(double x1, double y1, double x2, double y2)
{
	mat4 modelview1 = translate(*slCurrentMatrix, vec3(x1, y1, 0.0));
	mat4 modelview2 = translate(*slCurrentMatrix, vec3(x2, y2, 0.0));

	sliPointsFlush();
	sliTextFlush(*slCurrentMatrix, slForeColor);
	sliLine(slForeColor, modelview1[3][0], modelview1[3][1], modelview2[3][0], modelview2[3][1]);
}

void slSprite(const char *textureFilename, double x, double y, double width, double height)
{
	slSprite(textureFilename, x, y, width, height, 1.0, 1.0, 0.0, 0.0);
}

void slSprite(const char *textureFilename, double x, double y, double width, double height, double tilingX, double tilingY)
{
	slSprite(textureFilename, x, y, width, height, tilingX, tilingY, 0.0, 0.0);
}

void slSprite(const char *textureFilename, double x, double y, double width, double height, double tilingX, double tilingY, double scrollX, double scrollY)
{
	mat4 modelview;
	vec2 tiling = vec2(tilingX, tilingY);
	vec2 scroll = vec2(scrollX, scrollY);

	modelview = translate(*slCurrentMatrix, vec3(x, y, 0.0));
	modelview = scale(modelview, vec3(width, height, 1.0));

	sliPointsFlush();
	sliLinesFlush();
	sliTextFlush(*slCurrentMatrix, slForeColor);
	sliSprite(modelview, slForeColor, AssetManager::getTexture(textureFilename), tiling, scroll);
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

double slGetTextWidth(const char *text)
{
	return sliTextWidth(text);
}

double slGetTextHeight(const char *text)
{
	return sliTextHeight(text);
}

void slSetFont(const char *fontFilename, int fontSize)
{
	sliFont(fontFilename, fontSize);
}

void slText(double x, double y, const char *text)
{
	mat4 modelview = translate(*slCurrentMatrix, vec3(x, y, 0.0));

	if(slTextAlign == SL_ALIGN_CENTER)
	{
		modelview = translate(modelview, vec3(-slGetTextWidth(text) / 2.0, 0.0, 0.0));
	}
	else if(slTextAlign == SL_ALIGN_RIGHT)
	{
		modelview = translate(modelview, vec3(-slGetTextWidth(text), 0.0, 0.0));
	}

	sliPointsFlush();
	sliLinesFlush();
	sliText(modelview, slForeColor, text);
}

// private functions

void slInitResources()
{
	sliShadersInit(slProjectionMatrix);
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

}
