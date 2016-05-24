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

#define MAX_LINE_GROUP_SIZE 1000

#ifndef USE_GLES
	static GLuint sliLineVAO = 0;
#endif
static GLuint sliLineVBOs[2] = {0, 0};

static GLfloat *sliLineVertices;
static GLfloat *sliLineColors;

static int sliLineCount;
static GLfloat *sliLineVerticesPtr;
static GLfloat *sliLineColorsPtr;

void sliLineInit()
{
	// initialize our state object
	#ifndef USE_GLES
		glGenVertexArrays(1, &sliLineVAO);
		glBindVertexArray(sliLineVAO);
	#endif
	glGenBuffers(2, sliLineVBOs);

	// configure vertex data
	glBindBuffer(GL_ARRAY_BUFFER, sliLineVBOs[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * MAX_LINE_GROUP_SIZE * 4, NULL, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(0);

	// configure color data
	glBindBuffer(GL_ARRAY_BUFFER, sliLineVBOs[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * MAX_LINE_GROUP_SIZE * 8, NULL, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(1);

	// initialize our vertex data buffers
	sliLineVertices = (GLfloat*)malloc(sizeof(GLfloat) * MAX_LINE_GROUP_SIZE * 4);
	sliLineVerticesPtr = sliLineVertices;

	// initialize our color data buffers
	sliLineColors = (GLfloat*)malloc(sizeof(GLfloat) * MAX_LINE_GROUP_SIZE * 8);
	sliLineColorsPtr = sliLineColors;

	// no lines buffered yet
	sliLineCount = 0;
}

void sliLineDestroy()
{
	// free up dynamically-allocated vertex and color data
	free(sliLineVertices);
	free(sliLineColors);

	// free OpenGL objects
	glDeleteBuffers(2, sliLineVBOs);
	#ifndef USE_GLES
		glDeleteVertexArrays(1, &sliLineVAO);
	#endif
}

void sliLine(Vec4 *color, double x1, double y1, double x2, double y2)
{
	// assign vertex positions for incoming line
	*(sliLineVerticesPtr++) = x1;
	*(sliLineVerticesPtr++) = y1;
	*(sliLineVerticesPtr++) = x2;
	*(sliLineVerticesPtr++) = y2;

	// assign color values for the incoming point
	*(sliLineColorsPtr++) = color -> x;
	*(sliLineColorsPtr++) = color -> y;
	*(sliLineColorsPtr++) = color -> z;
	*(sliLineColorsPtr++) = color -> w;

	// assign color values for the incoming point
	*(sliLineColorsPtr++) = color -> x;
	*(sliLineColorsPtr++) = color -> y;
	*(sliLineColorsPtr++) = color -> z;
	*(sliLineColorsPtr++) = color -> w;

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

		// send vertex positions to graphics card
		glBindBuffer(GL_ARRAY_BUFFER, sliLineVBOs[0]);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(GLfloat) * sliLineCount * 4, sliLineVertices);
		#ifdef USE_GLES
			glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, NULL);
		#endif

		// send vertex colors to graphics card
		glBindBuffer(GL_ARRAY_BUFFER, sliLineVBOs[1]);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(GLfloat) * sliLineCount * 8, sliLineColors);
		#ifdef USE_GLES
			glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, NULL);
		#endif

		// prepare our shader object
		shaderBind(sliPointShader);

		// render the lines
		glDrawArrays(GL_LINES, 0, sliLineCount * 2);

		// reset the lines buffer
		sliLineVerticesPtr = sliLineVertices;
		sliLineColorsPtr = sliLineColors;
		sliLineCount = 0;
	}
}
