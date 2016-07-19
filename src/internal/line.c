#include "line.h"
#include "shaders.h"

#include "../util/shader.h"
#include "../util/transform.h"

#include "config.h"

#ifdef USE_GLES
	#include <GLES2/gl2.h>
	#include <GLES2/gl2ext.h>
#else
	#include <GL/glew.h>
#endif

#include <stdlib.h>

#define MAX_LINE_GROUP_SIZE 1024

#ifndef USE_GLES
	static GLuint sliLineVAO = 0;
#endif
static GLuint sliLineVBOs[1] = {0};

static GLfloat *sliLineAttributes;

static int sliLineCount;
static GLfloat *sliLineAttributesPtr;

void sliLineInit()
{
	// initialize our state object
	#ifndef USE_GLES
		glGenVertexArrays(1, &sliLineVAO);
		glBindVertexArray(sliLineVAO);
	#endif
	glGenBuffers(1, sliLineVBOs);

	// configure vertex attribute data
	glBindBuffer(GL_ARRAY_BUFFER, sliLineVBOs[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * MAX_LINE_GROUP_SIZE * 12, NULL, GL_DYNAMIC_DRAW);

	// configure vertex position attribute
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 6, NULL);
	glEnableVertexAttribArray(0);

	// configure vertex color attribute
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 6, (GLvoid*)(sizeof(GLfloat) * 2));
	glEnableVertexAttribArray(1);

	// initialize our vertex data buffer
	sliLineAttributes = (GLfloat*)malloc(sizeof(GLfloat) * MAX_LINE_GROUP_SIZE * 12);
	sliLineAttributesPtr = sliLineAttributes;

	// no lines buffered yet
	sliLineCount = 0;
}

void sliLineDestroy()
{
	// free up dynamically-allocated vertex and color data
	free(sliLineAttributes);

	// free OpenGL objects
	glDeleteBuffers(1, sliLineVBOs);
	#ifndef USE_GLES
		glDeleteVertexArrays(1, &sliLineVAO);
	#endif
}

void sliLine(Vec4 *color, double x1, double y1, double x2, double y2)
{
	// assign vertex positions for first point of incoming line
	*(sliLineAttributesPtr++) = (float)x1;
	*(sliLineAttributesPtr++) = (float)y1;

	// assign color values for first point of incoming line
	*(sliLineAttributesPtr++) = (float)color -> x;
	*(sliLineAttributesPtr++) = (float)color -> y;
	*(sliLineAttributesPtr++) = (float)color -> z;
	*(sliLineAttributesPtr++) = (float)color -> w;

	// assign vertex positions for second point of incoming line
	*(sliLineAttributesPtr++) = (float)x2;
	*(sliLineAttributesPtr++) = (float)y2;

	// assign color values for second point of incoming line
	*(sliLineAttributesPtr++) = (float)color -> x;
	*(sliLineAttributesPtr++) = (float)color -> y;
	*(sliLineAttributesPtr++) = (float)color -> z;
	*(sliLineAttributesPtr++) = (float)color -> w;

	// track the number of lines we've cached; render if the buffer is full
	sliLineCount ++;
	if(sliLineCount >= MAX_LINE_GROUP_SIZE)
	{
		sliLinesFlush();
	}
}

void sliLinesFlush()
{
	// nothing to do if no lines are waiting to be drawn
	if(sliLineCount > 0)
	{
		// activate our state object
		#ifndef USE_GLES
			glBindVertexArray(sliLineVAO);
		#endif

		// send vertex attributes to graphics card
		glBindBuffer(GL_ARRAY_BUFFER, sliLineVBOs[0]);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(GLfloat) * sliLineCount * 12, sliLineAttributes);
		#ifdef USE_GLES
			glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 6, NULL);
			glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 6, (GLvoid*)(sizeof(GLfloat) * 2));
		#endif

		// prepare our shader object
		shaderBind(sliPointShader);

		// render the lines
		glDrawArrays(GL_LINES, 0, sliLineCount * 2);

		// reset the lines buffer
		sliLineAttributesPtr = sliLineAttributes;
		sliLineCount = 0;
	}
}
