#include "GLFW/glfw3.h"

#include <stdio.h>

#ifdef __MINGW32__
	#include <windows.h>
#endif

static GLFWwindow *sliProgramWindow = NULL;
static int sliWindowWidth = 0;
static int sliWindowHeight = 0;

void sliOpenWindow(int width, int height, const char *title, int fullScreen)
{
	// types enabling us to access WGL functionality for enabling vsync in Windows
	//#ifdef __MINGW32__
		//typedef BOOL (WINAPI *PFNWGLSWAPINTERVALEXTPROC)(int interval);
		//PFNWGLSWAPINTERVALEXTPROC wglSwapIntervalEXT = NULL;
    //#endif

	// start up GLFW
	glfwInit();

	// set our OpenGL context to something that doesn't support any old-school shit
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_REFRESH_RATE, 60);

	// create our OpenGL window
	sliProgramWindow = glfwCreateWindow(width, height, title, fullScreen ? glfwGetPrimaryMonitor() : NULL, NULL);
	if (sliProgramWindow == NULL) {
		fprintf(stderr, "Failed to create the window. Make sure your system supports OpenGL 3.2\n");
		exit(1);
	}
	glfwMakeContextCurrent(sliProgramWindow);
	glfwSwapInterval(1);

	// record window size
	sliWindowWidth = width;
	sliWindowHeight = height;

	// GLFW doesn't handle vsync well in all cases, so we have to go straight to WGL to do this
	//#ifdef __MINGW32__
		//wglSwapIntervalEXT = (PFNWGLSWAPINTERVALEXTPROC)wglGetProcAddress("wglSwapIntervalEXT");
		//wglSwapIntervalEXT(1);
	//#endif

	// enable OpenGL debugging context if we're in a debug build
	#ifdef DEBUG
		glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
	#endif
}

void sliShowCursor(int showCursor)
{
	glfwSetInputMode(sliProgramWindow, GLFW_CURSOR, showCursor ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_HIDDEN);
}

void sliCloseWindow()
{
	glfwDestroyWindow(sliProgramWindow);
	glfwTerminate();
	sliProgramWindow = NULL;
}

int sliIsWindowOpen()
{
	return sliProgramWindow != NULL;
}

int sliShouldClose()
{
	return glfwWindowShouldClose(sliProgramWindow);
}

int sliGetKey(int key)
{
	return glfwGetKey(sliProgramWindow, key) == GLFW_PRESS;
}

int sliGetMouseButton(int button)
{
	return glfwGetMouseButton(sliProgramWindow, button) == GLFW_PRESS;
}

void sliGetMousePos(int *posX, int *posY)
{
	double x, y;
	glfwGetCursorPos(sliProgramWindow, &x, &y);
	*posX = (int)x;
	*posY = sliWindowHeight - (int)y;
}

double sliGetTime()
{
	return glfwGetTime();
}

void sliPollAndSwap()
{
	glfwPollEvents();
	glfwSwapBuffers(sliProgramWindow);
}
