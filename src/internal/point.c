#include "point.h"
#include "shaders.h"

#include "../util/shader.h"

#include "config.h"

#ifdef USE_GLES
	#include <GLES2/gl2.h>
	#include <GLES2/gl2ext.h>
#else
	#include <GL/glew.h>
#endif

#include <stdlib.h>

#define MAX_POINT_GROUP_SIZE 1000

#ifndef USE_GLES
	static GLuint sliPointVAO = 0;
#endif
static GLuint sliPointVBOs[2] = {0, 0};

static GLfloat *sliPointVertices;
static GLfloat *sliPointColors;

static int sliPointCount;
static GLfloat *sliPointVerticesPtr;
static GLfloat *sliPointColorsPtr;

void sliPointInit()
{
	// initialize our state object
	#ifndef USE_GLES
		glGenVertexArrays(1, &sliPointVAO);
		glBindVertexArray(sliPointVAO);
	#endif
	glGenBuffers(2, sliPointVBOs);

	// configure vertex data
	glBindBuffer(GL_ARRAY_BUFFER, sliPointVBOs[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * MAX_POINT_GROUP_SIZE * 2, NULL, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(0);

	// configure color data
	glBindBuffer(GL_ARRAY_BUFFER, sliPointVBOs[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * MAX_POINT_GROUP_SIZE * 4, NULL, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(1);

	// initialize our vertex data buffers
	sliPointVertices = (GLfloat*)malloc(sizeof(GLfloat) * MAX_POINT_GROUP_SIZE * 2);
	sliPointVerticesPtr = sliPointVertices;

	// initialize our color data buffers
	sliPointColors = (GLfloat*)malloc(sizeof(GLfloat) * MAX_POINT_GROUP_SIZE * 4);
	sliPointColorsPtr = sliPointColors;

	// no points buffered yet
	sliPointCount = 0;
}

void sliPointDestroy()
{
	// free up dynamically-allocated vertex and color data
	free(sliPointVertices);
	free(sliPointColors);

	// free OpenGL objects
	glDeleteBuffers(2, sliPointVBOs);
	#ifndef USE_GLES
		glDeleteVertexArrays(1, &sliPointVAO);
	#endif
}

void sliPoint(Mat4 *modelview, Vec4 *color)
{
	// assign vertex position for the incoming point
	*(sliPointVerticesPtr++) = (float)modelview -> cols[3].x;
	*(sliPointVerticesPtr++) = (float)modelview -> cols[3].y;

	// assign color values for the incoming point
	*(sliPointColorsPtr++) = (float)color -> x;
	*(sliPointColorsPtr++) = (float)color -> y;
	*(sliPointColorsPtr++) = (float)color -> z;
	*(sliPointColorsPtr++) = (float)color -> w;

	// track the number of points we've cached; render if the buffer is full
	sliPointCount ++;
	if(sliPointCount >= MAX_POINT_GROUP_SIZE)
	{
		sliPointsFlush();
	}
}

void sliPointsFlush()
{
	// nothing to do if no points are waiting to be drawn
	if(sliPointCount > 0)
	{
		// activate our state object
		#ifndef USE_GLES
			glBindVertexArray(sliPointVAO);
		#endif

		// send vertex positions to graphics card
		glBindBuffer(GL_ARRAY_BUFFER, sliPointVBOs[0]);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(GLfloat) * sliPointCount * 2, sliPointVertices);
		#ifdef USE_GLES
			glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, NULL);
		#endif

		// send vertex colors to graphics card
		glBindBuffer(GL_ARRAY_BUFFER, sliPointVBOs[1]);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(GLfloat) * sliPointCount * 4, sliPointColors);
		#ifdef USE_GLES
			glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, NULL);
		#endif

		// prepare our shader object
		shaderBind(sliPointShader);

		// render the points
		#ifndef USE_GLES
			glPointSize(1.0);
		#endif
		glDrawArrays(GL_POINTS, 0, sliPointCount);

		// reset the points buffer
		sliPointVerticesPtr = sliPointVertices;
		sliPointColorsPtr = sliPointColors;
		sliPointCount = 0;
	}
}
