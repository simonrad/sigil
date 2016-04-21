#include "util/gldebugging.h"

#include "gl/glew.h"

#include <stdio.h>
#include <assert.h>
#include <windows.h>

// thanks to:
// https://blog.nobel-joergensen.com/2013/02/17/debugging-opengl-part-2-using-gldebugmessagecallback/

static void APIENTRY openglCallbackFunction(GLenum source,
                                            GLenum type,
                                            GLuint id,
                                            GLenum severity,
                                            GLsizei length,
                                            const GLchar* message,
                                            const void* userParam)
{
    printf("---------------------OPENGL MESSAGE DETECTED------------\n");
    printf("message: %s\n", message);
    printf("type: ");
    switch (type)
    {
    case GL_DEBUG_TYPE_ERROR:
        printf("ERROR");
        break;
    case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
        printf("DEPRECATED_BEHAVIOR");
        break;
    case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
        printf("UNDEFINED_BEHAVIOR");
        break;
    case GL_DEBUG_TYPE_PORTABILITY:
        printf("PORTABILITY");
        break;
    case GL_DEBUG_TYPE_PERFORMANCE:
        printf("PERFORMANCE");
        break;
    case GL_DEBUG_TYPE_OTHER:
        printf("OTHER");
        break;
    }
    printf("\n");

    printf("id: %d\n", (int)id);
    printf("severity: ");
    switch (severity)
    {
    case GL_DEBUG_SEVERITY_LOW:
        printf("LOW");
        break;
    case GL_DEBUG_SEVERITY_MEDIUM:
        printf("MEDIUM");
        break;
    case GL_DEBUG_SEVERITY_HIGH:
        printf("HIGH");
        break;
    }
    printf("\n");
}

void initGLDebugger()
{
	GLuint unusedIds = 0;

	// make sure message callback debugging is supported
	if(glDebugMessageCallback)
	{
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        glDebugMessageCallback(openglCallbackFunction, NULL);
        glDebugMessageControl(GL_DONT_CARE,
							  GL_DONT_CARE,
							  GL_DONT_CARE,
							  0,
							  &unusedIds,
							  GL_TRUE);
		printf("OPENGL CALLBACK DEBUGGING IS ENABLED\n");
    }
    else
    {
        printf("OPENGL CALLBACK DEBUGGING IS NOT AVAILABLE\n");
	}
}
