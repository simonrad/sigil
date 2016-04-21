#include "internal/point.h"
#include "internal/shaders.h"

#include "util/shader.h"

#include <stdlib.h>

#include "gl/glew.h"

#define MAX_POINT_GROUP_SIZE 1000

static GLuint sliPointVAO = 0;
static GLuint sliPointVBOs[2] = {0, 0};

static GLfloat *sliPointVertices;
static GLfloat *sliPointColors;

static int sliPointCount;
static GLfloat *sliPointVerticesPtr;
static GLfloat *sliPointColorsPtr;

void sliPointInit()
{
	// initialize our state object
	glGenVertexArrays(1, &sliPointVAO);
	glBindVertexArray(sliPointVAO);
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
	glDeleteVertexArrays(1, &sliPointVAO);
}

void sliPoint(Mat4 *modelview, Vec4 *color)
{
	// assign vertex position for the incoming point
	*(sliPointVerticesPtr++) = modelview -> cols[3].x;
	*(sliPointVerticesPtr++) = modelview -> cols[3].y;

	// assign color values for the incoming point
	*(sliPointColorsPtr++) = color -> x;
	*(sliPointColorsPtr++) = color -> y;
	*(sliPointColorsPtr++) = color -> z;
	*(sliPointColorsPtr++) = color -> w;

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
		glBindVertexArray(sliPointVAO);

		// send vertex positions to graphics card
		glBindBuffer(GL_ARRAY_BUFFER, sliPointVBOs[0]);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(GLfloat) * sliPointCount * 2, sliPointVertices);

		// send vertex colors to graphics card
		glBindBuffer(GL_ARRAY_BUFFER, sliPointVBOs[1]);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(GLfloat) * sliPointCount * 4, sliPointColors);

		// prepare our shader object
		shaderBind(sliPointShader);

		// render the points
		glPointSize(1.0);
		glDrawArrays(GL_POINTS, 0, sliPointCount);

		// reset the points buffer
		sliPointVerticesPtr = sliPointVertices;
		sliPointColorsPtr = sliPointColors;
		sliPointCount = 0;
	}
}
