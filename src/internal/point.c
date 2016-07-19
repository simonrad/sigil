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

#define MAX_POINT_GROUP_SIZE 1024

#ifndef USE_GLES
	static GLuint sliPointVAO = 0;
#endif
static GLuint sliPointVBOs[1] = {0};

static GLfloat *sliPointAttributes;

static int sliPointCount;
static GLfloat *sliPointAttributesPtr;

void sliPointInit()
{
	// initialize our state object
	#ifndef USE_GLES
		glGenVertexArrays(1, &sliPointVAO);
		glBindVertexArray(sliPointVAO);
	#endif
	glGenBuffers(1, sliPointVBOs);

	// configure vertex attribute data
	glBindBuffer(GL_ARRAY_BUFFER, sliPointVBOs[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * MAX_POINT_GROUP_SIZE * 6, NULL, GL_DYNAMIC_DRAW);

	// configure vertex position attribute
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 6, NULL);
	glEnableVertexAttribArray(0);

	// configure vertex color attribute
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 6, (GLvoid*)(sizeof(GLfloat) * 2));
	glEnableVertexAttribArray(1);

	// initialize our vertex data buffer
	sliPointAttributes = (GLfloat*)malloc(sizeof(GLfloat) * MAX_POINT_GROUP_SIZE * 6);
	sliPointAttributesPtr = sliPointAttributes;

	// no points buffered yet
	sliPointCount = 0;
}

void sliPointDestroy()
{
	// free up dynamically-allocated vertex and color data
	free(sliPointAttributes);

	// free OpenGL objects
	glDeleteBuffers(1, sliPointVBOs);
	#ifndef USE_GLES
		glDeleteVertexArrays(1, &sliPointVAO);
	#endif
}

void sliPoint(Mat4 *modelview, Vec4 *color)
{
	// assign vertex position for the incoming point
	*(sliPointAttributesPtr++) = (float)modelview -> cols[3].x;
	*(sliPointAttributesPtr++) = (float)modelview -> cols[3].y;

	// assign color values for the incoming point
	*(sliPointAttributesPtr++) = (float)color -> x;
	*(sliPointAttributesPtr++) = (float)color -> y;
	*(sliPointAttributesPtr++) = (float)color -> z;
	*(sliPointAttributesPtr++) = (float)color -> w;

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
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(GLfloat) * sliPointCount * 6, sliPointAttributes);
		#ifdef USE_GLES
			glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 6, NULL);
			glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 6, (GLvoid*)(sizeof(GLfloat) * 2));
		#endif

		// prepare our shader object
		shaderBind(sliPointShader);

		// render the points
		#ifndef USE_GLES
			glPointSize(1.0);
		#endif
		glDrawArrays(GL_POINTS, 0, sliPointCount);

		// reset the points buffer
		sliPointAttributesPtr = sliPointAttributes;
		sliPointCount = 0;
	}
}
