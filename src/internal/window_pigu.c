#include "pigu.h"

static int sliWindowOpen = 0;
static int sliWindowWidth = 0;
static int sliWindowHeight = 0;

void sliOpenWindow(int width, int height, const char *title, int fullScreen)
{
	// start up PIGU
	piguInit();

	// create our OpenGL window; fullScreen parameter is ignored
	piguCreateWindow(width,					// window width
					 height,				// window height
					 8,						// red bits
					 8,						// green bits
					 8,						// blue bits
					 8,						// alpha bits
					 24,					// depth bits
					 8,						// stencil bits
					 0);					// no multi-sampling

	// record window size
	sliWindowWidth = width;
	sliWindowHeight = height;

	// record the window as open
	sliWindowOpen = 1;
}

void sliShowCursor(int showCursor) { }

void sliCloseWindow()
{
	piguCloseWindow();
	sliWindowOpen = 0;
}

int sliIsWindowOpen()
{
	return sliWindowOpen;
}

int sliShouldClose()
{
	return 0;
}

int sliGetKey(int key)
{
	return piguIsKeyDown(key);
}

int sliGetMouseButton(int button)
{
	return piguIsMouseButtonDown(button);
}

void sliGetMousePos(int *posX, int *posY)
{
	piguGetMousePosition(posX, posY);
	*posY = sliWindowHeight - *posY;
}

double sliGetTime()
{
	return piguGetTime();
}

void sliPollAndSwap()
{
	piguPollEvents();
	piguSwapBuffers();
}
