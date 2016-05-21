#include "pigu.h"

static inr sliWindowOpen = 0;
static int sliWindowWidth = 0;
static int sliWindowHeight = 0;

void sliOpenWindow(int width, int height, const char *title)
{
	// start up PIGU
	piguInit();

	// create our OpenGL window
	piguCreateWindow(width,					// window width
					 height,				// window height
					 8,						// red bits
					 8,						// green bits
					 8,						// blue bits
					 8,						// alpha bits
					 24,					// depth bits
					 8,						// stencil bits
					 0);					// samples (although don't ask what kind...PIGU doesn't specify)

	// record window size
	sliWindowWidth = width;
	sliWindowHeight = height;

	// record the window as open
	sliWindowOpen = 1;
}

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

int sliGetKey()
{
	return piguIsKeyDown(key);
}

int sliGetMouseButton()
{
	return piguIsMouseButtonDown(key);
}

void sliGetMousePos(int *posX, int *posY)
{
	piguGetMousePosition(sliProgramWindow, posX, posY);
	*posY = sliWindowHeight - posY;
}

double sliGetTime()
{
	return piguGetTime();
}

void sliPollAndSwap()
{
	glfwPollEvents();
	glfwSwapBuffers(slProgramWindow);
}
