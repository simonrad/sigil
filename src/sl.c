#include "sl.h"

#include "internal/shaders.h"
#include "internal/triangle.h"
#include "internal/rectangle.h"
#include "internal/circle.h"
#include "internal/point.h"
#include "internal/line.h"
#include "internal/sprite.h"
#include "internal/text.h"
#include "internal/sound.h"
#include "internal/window.h"

#include "util/transform.h"
#include "util/images.h"

#include "config.h"

#ifdef __MINGW32__
	#include "util/gldebugging.h"
#endif

#ifdef USE_GLES
	#include <GLES2/gl2.h>
	#include <GLES2/gl2ext.h>
#else
	#include <GL/glew.h>
#endif

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#define SL_MATRIX_STACK_SIZE 32

#define IDEAL_FRAME_TIME 0.01666667

// private vars

static int slMousePosX;
static int slMousePosY;
static uint8_t slMousePosStale = 1;

static Mat4 slMatrixStack[SL_MATRIX_STACK_SIZE];
static Mat4 *slCurrentMatrix = &slMatrixStack[0];
static int slStackSize = 0;

static Mat4 slProjectionMatrix;

static Vec4 slForeColor;// = {.x = 1.0, .y = 1.0, .z = 1.0, .w = 1.0};

static double slSpriteScrollX = 0.0;
static double slSpriteScrollY = 0.0;
static double slSpriteTilingX = 1.0;
static double slSpriteTilingY = 1.0;

static int slTextAlign = SL_ALIGN_LEFT;

static double slDeltaTime = IDEAL_FRAME_TIME;
static double slOldFrameTime = 0.0;
static double slNewFrameTime = IDEAL_FRAME_TIME;

// private function prototypes

static void slInitResources();
static void slKillResources();

// window commands

void slWindow(int width, int height, const char *title)
{
	// error tracking for any window-creation issues we run into
	#ifndef USE_GLES
		GLenum error;
	#endif

	if(!sliIsWindowOpen())
	{
		// use either GLFW or PIGU to set up our window
		sliOpenWindow(width, height, title);

		// configure our viewing area
		glViewport(0, 0, width, height);

		// enable our extensions handler
		#ifndef USE_GLES
			glewExperimental = 1;
			error = glewInit();
			if(error != GLEW_OK)
			{
				fprintf(stderr, "slWindow() could not initialize GLEW: %s\n", glewGetErrorString(error));
				exit(1);
			}
		#endif

		// start with a clean error slate
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
		slProjectionMatrix = ortho(0.0f, (double)width, 0.0f, (double)height);

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
		fprintf(stderr, "slWindow() cannot be called when a window already exists\n");
		exit(1);
	}
}

void slClose()
{
	if(sliIsWindowOpen())
	{
		slKillResources();
		sliCloseWindow();
	}
	else
	{
		fprintf(stderr, "slClose() cannot be called when no window exists\n");
		exit(1);
	}
}

int slShouldClose()
{
	if(!sliIsWindowOpen())
	{
		fprintf(stderr, "slShouldClose() cannot be called because no window exists\n");
		exit(1);
	}

	return sliShouldClose();
}

// simple input

int slGetKey(int key)
{
	return sliGetKey(key);
}

int slGetMouseButton(int button)
{
	return sliGetMouseButton(button);
}

int slGetMouseX()
{
	// make sure a render step has not occurred since we last read the mouse position
	if(slMousePosStale)
	{
		sliGetMousePos(&slMousePosX, &slMousePosY);
		slMousePosStale = 0;
	}

	return slMousePosX;
}

int slGetMouseY()
{
	// make sure a render step has not occurred since we last read the mouse position
	if(slMousePosStale)
	{
		sliGetMousePos(&slMousePosX, &slMousePosY);
		slMousePosStale = 0;
	}

	return slMousePosY;
}

// simple frame timing

double slGetDeltaTime()
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
	sliPollAndSwap();
	glClear(GL_COLOR_BUFFER_BIT);

	// gather time values
	slOldFrameTime = slNewFrameTime;
	slNewFrameTime = sliGetTime();

	// compute delta time value; ensure we don't have any long pauses or tiny time quantums
	slDeltaTime = (slNewFrameTime - slOldFrameTime);
	if(slDeltaTime < SL_MIN_DELTA_TIME)
		slDeltaTime = SL_MIN_DELTA_TIME;
	if(slDeltaTime > SL_MAX_DELTA_TIME)
		slDeltaTime = SL_MAX_DELTA_TIME;

	// set our mouse position as needing refreshing
	slMousePosStale = 1;
}

// colour control

void slSetBackColor(double red, double green, double blue)
{
	glClearColor((GLclampf)red, (GLclampf)green, (GLclampf)blue, 1.0);
}

void slSetForeColor(double red, double green, double blue, double alpha)
{
	slForeColor.x = (float)red;
	slForeColor.y = (float)green;
	slForeColor.z = (float)blue;
	slForeColor.w = (float)alpha;
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
	if(slStackSize < SL_MATRIX_STACK_SIZE - 1)
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

void slTranslate(double x, double y)
{
	*slCurrentMatrix = translate(slCurrentMatrix, x, y);
}

void slRotate(double degrees)
{
	*slCurrentMatrix = rotate(slCurrentMatrix, degrees);
}

void slScale(double x, double y)
{
	*slCurrentMatrix = scale(slCurrentMatrix, x, y);
}

// texture loading

int slLoadTexture(const char *filename)
{
	int result = -1;

	if(sliIsWindowOpen())
	{
		result = (int)loadOpenGLTexture(filename);
	}
	else
	{
		fprintf(stderr, "slLoadTexture() cannot be called before slWindow() is called\n");
		exit(1);
	}

	return result;
}

// sound loading and playing

int slLoadWAV(const char *filename)
{
	int result =  -1;

	if(sliIsWindowOpen())
	{
		result = sliLoadWAV(filename);
	}
	else
	{
		fprintf(stderr, "slLoadWAV() cannot be called before slWindow() is called\n");
		exit(1);
	}

	return result;
}

int slSoundPlay(int sound)
{
	return sliSoundPlay(sound);
}

int slSoundLoop(int sound)
{
	return sliSoundLoop(sound);
}

void slSoundPause(int sound)
{
	sliSoundPause(sound);
}

void slSoundStop(int sound)
{
	sliSoundStop(sound);
}

void slSoundPauseAll()
{
	sliSoundPauseAll();
}

void slSoundResumeAll()
{
	sliSoundResumeAll();
}

void slSoundStopAll()
{
	sliSoundStopAll();
}

int slSoundPlaying(int sound)
{
	return sliSoundPlaying(sound);
}

int slSoundLooping(int sound)
{
	return sliSoundLooping(sound);
}

// simple shape commands

void slTriangleFill(double x, double y, double width, double height)
{
	Mat4 modelview = translate(slCurrentMatrix, x, y);
	modelview = scale(&modelview, width, height);

	sliPointsFlush();
	sliLinesFlush();
	sliTextFlush(slCurrentMatrix, &slForeColor);
	sliTriangleFill(&modelview, &slForeColor);
}

void slTriangleOutline(double x, double y, double width, double height)
{
	Mat4 modelview = translate(slCurrentMatrix, x, y);
	modelview = scale(&modelview, width, height);

	sliPointsFlush();
	sliLinesFlush();
	sliTextFlush(slCurrentMatrix, &slForeColor);
	sliTriangleOutline(&modelview, &slForeColor);
}

void slRectangleFill(double x, double y, double width, double height)
{
	Mat4 modelview = translate(slCurrentMatrix, x, y);
	modelview = scale(&modelview, width, height);

	sliPointsFlush();
	sliLinesFlush();
	sliTextFlush(slCurrentMatrix, &slForeColor);
	sliRectangleFill(&modelview, &slForeColor);
}

void slRectangleOutline(double x, double y, double width, double height)
{
	Mat4 modelview = translate(slCurrentMatrix, x, y);
	modelview = scale(&modelview, width, height);

	sliPointsFlush();
	sliLinesFlush();
	sliTextFlush(slCurrentMatrix, &slForeColor);
	sliRectangleOutline(&modelview, &slForeColor);
}

void slCircleFill(double x, double y, double radius, int numVertices)
{
	Mat4 modelview = translate(slCurrentMatrix, x, y);

	sliPointsFlush();
	sliLinesFlush();
	sliTextFlush(slCurrentMatrix, &slForeColor);
	sliCircleFill(&modelview, &slForeColor, radius, numVertices);
}

void slCircleOutline(double x, double y, double radius, int numVertices)
{
	Mat4 modelview = translate(slCurrentMatrix, x, y);

	sliPointsFlush();
	sliLinesFlush();
	sliTextFlush(slCurrentMatrix, &slForeColor);
	sliCircleOutline(&modelview, &slForeColor, radius, numVertices);
}

void slPoint(double x, double y)
{
	Mat4 modelview = translate(slCurrentMatrix, x, y);

	sliLinesFlush();
	sliTextFlush(slCurrentMatrix, &slForeColor);
	sliPoint(&modelview, &slForeColor);
}

void slLine(double x1, double y1, double x2, double y2)
{
	Mat4 modelview1 = translate(slCurrentMatrix, x1, y1);
	Mat4 modelview2 = translate(slCurrentMatrix, x2, y2);

	sliPointsFlush();
	sliTextFlush(slCurrentMatrix, &slForeColor);
	sliLine(&slForeColor, modelview1.cols[3].x, modelview1.cols[3].y, modelview2.cols[3].x, modelview2.cols[3].y);
}

void slSetSpriteTiling(double x, double y)
{
	slSpriteTilingX = x;
	slSpriteTilingY = y;
}

void slSetSpriteScroll(double x, double y)
{
	slSpriteScrollX = x;
	slSpriteScrollY = y;
}

void slSprite(int texture, double x, double y, double width, double height)
{
	Mat4 modelview;

	// this shorthand causes compiler errors on MSVC...
	Vec2 tiling;// = {.x = slSpriteTilingX, .y = slSpriteTilingY};
	Vec2 scroll;// = {.x = slSpriteScrollX, .y = slSpriteScrollY};

	// ...so we do it the hard way instead
	tiling.x = (float)slSpriteTilingX;
	tiling.y = (float)slSpriteTilingY;
	scroll.x = (float)slSpriteScrollX;
	scroll.y = (float)slSpriteScrollY;

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
	if(sliIsWindowOpen())
	{
		sliFont(fontFilename, fontSize);
	}
	else
	{
		fprintf(stderr, "slSetFont() cannot be called before slWindow() is called\n");
		exit(1);
	}
}

void slSetFontSize(int fontSize)
{
	sliFontSize(fontSize);
}

void slText(double x, double y, const char *text)
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
	sliSoundInit();
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
	sliSoundDestroy();
}
