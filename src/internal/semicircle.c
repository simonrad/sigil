#include "semicircle.h"
#include "shaders.h"

#include "../util/shader.h"

#include "config.h"

#ifdef USE_GLES
	#include <GLES2/gl2.h>
	#include <GLES2/gl2ext.h>
#else
	#include <GL/glew.h>
#endif

#include <math.h>
#include <stdlib.h>

#define PI 3.1415926
#define MIN_VERTICES 3
#define MAX_VERTICES 100

#ifndef USE_GLES
	static GLuint sliSemiCircleOutlineVAO = 0;
#endif
static GLuint sliSemiCircleOutlineVBOs[1] = {0};

#ifndef USE_GLES
	static GLuint sliSemiCircleFillVAO = 0;
#endif
static GLuint sliSemiCircleFillVBOs[1] = {0};

static double sliOutlineRadius = 0;
static double sliFillRadius = 0;

static int sliNumOutlineVertices = 0;
static int sliNumFillVertices = 0;

void sliSemiCircleInit()
{
	// initialize our outline state object
	#ifndef USE_GLES
		glGenVertexArrays(1, &sliSemiCircleOutlineVAO);
		glBindVertexArray(sliSemiCircleOutlineVAO);
	#endif
	glGenBuffers(1, sliSemiCircleOutlineVBOs);

	// configure outline vertex data
	glBindBuffer(GL_ARRAY_BUFFER, sliSemiCircleOutlineVBOs[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * (MAX_VERTICES + 1) * 2, NULL, GL_DYNAMIC_DRAW);		// + 1 for middle of line loop
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(0);

	// initialize our fill state object
	#ifndef USE_GLES
		glGenVertexArrays(1, &sliSemiCircleFillVAO);
		glBindVertexArray(sliSemiCircleFillVAO);
	#endif
	glGenBuffers(1, sliSemiCircleFillVBOs);

	// configure fill vertex data
	glBindBuffer(GL_ARRAY_BUFFER, sliSemiCircleFillVBOs[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * (MAX_VERTICES + 1) * 2, NULL, GL_DYNAMIC_DRAW);		// + 1 for middle of triangle fan
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(0);
}

void sliSemiCircleDestroy()
{
	glDeleteBuffers(1, sliSemiCircleOutlineVBOs);
	#ifndef USE_GLES
		glDeleteVertexArrays(1, &sliSemiCircleOutlineVAO);
	#endif

	glDeleteBuffers(1, sliSemiCircleFillVBOs);
	#ifndef USE_GLES
		glDeleteVertexArrays(1, &sliSemiCircleFillVAO);
	#endif
}

void sliSemiCircleOutline(Mat4 *modelview, Vec4 *color, double radius, int numVertices, double degrees)
{
	const double EPS = 0.00001;

	GLfloat vertices[(MAX_VERTICES + 1) * 2];				// + 1 for middle of line loop
	GLfloat *vertexPtr;
	double theta;
	double transform;
	double c, s;
	double x, y;
	int i;

	// make sure the number of vertices is something reasonable
	if(numVertices < MIN_VERTICES) numVertices = MIN_VERTICES;
	if(numVertices > MAX_VERTICES) numVertices = MAX_VERTICES;

	// if the number of vertices specified is different from what we've allocated, prepare to re-compute them
	if(numVertices != sliNumOutlineVertices || fabs(radius - sliOutlineRadius) > EPS)
	{
		// clamp the degree range to something sane
		if(degrees < -360.0) degrees = 360.0;
		if(degrees > 360.0) degrees = 360.0;

		// assign new vertex count and radius
		sliNumOutlineVertices = numVertices;
		sliOutlineRadius = radius;
		vertexPtr = vertices;

		// compute angle increment, and pre-compute sin and cos of that increment
		theta = ((degrees / 180.0) * PI) / (double)(sliNumOutlineVertices - 1);
		c = cos(theta);
		s = sin(theta);

		// first vertex position of line loop
		x = radius;
		y = 0;

		// add center vertex for the middle point (the center of the loop)
		*vertexPtr++ = 0.0f;
		*vertexPtr++ = 0.0f;

		// compute new vertex positions
		for(i = 0; i < sliNumOutlineVertices; i ++)
		{
			*vertexPtr++ = (float)x;
			*vertexPtr++ = (float)y;

			// apply rotation matrix to current vertex position
			transform = x;
			x = c * x - s * y;
			y = s * transform + c * y;
		}

		// now pass the vertex data to OpenGL
		#ifndef USE_GLES
			glBindVertexArray(sliSemiCircleOutlineVAO);
		#endif
		glBindBuffer(GL_ARRAY_BUFFER, sliSemiCircleOutlineVBOs[0]);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(GLfloat) * (sliNumOutlineVertices + 1) * 2, vertices);		// + 1 for middle of line loop
	}

	// prepare our shader object
	shaderBind(sliBasicShader);
	shaderUniformMatrix4fv(sliBasicShader, "u_Modelview", 1, (float*)modelview);
	shaderUniform4f(sliBasicShader, "u_Color", (float)color -> x, (float)color -> y, (float)color -> z, (float)color -> w);

	// bind appropriate object state and render the object
	#ifndef USE_GLES
		glBindVertexArray(sliSemiCircleOutlineVAO);
	#else
		glBindBuffer(GL_ARRAY_BUFFER, sliSemiCircleOutlineVBOs[0]);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	#endif
	glDrawArrays(GL_LINE_LOOP, 0, sliNumOutlineVertices + 1);	// + 1 for middle of line loop
}

void sliSemiCircleFill(Mat4 *modelview, Vec4 *color, double radius, int numVertices, double degrees)
{
	const double EPS = 0.00001;

	GLfloat vertices[(MAX_VERTICES + 1) * 2];					// + 1 for middle of triangle fan
	GLfloat *vertexPtr;
	double theta;
	double transform;
	double c, s;
	double x, y;
	int i;

	// make sure the number of vertices is something reasonable
	if(numVertices < MIN_VERTICES) numVertices = MIN_VERTICES;
	if(numVertices > MAX_VERTICES) numVertices = MAX_VERTICES;

	// if the number of vertices specified is different from what we've allocated, prepare to re-compute them
	if(numVertices != sliNumFillVertices || fabs(radius - sliFillRadius) > EPS)
	{
		// clamp the degree range to something sane
		if(degrees < -360.0) degrees = 360.0;
		if(degrees > 360.0) degrees = 360.0;

		// assign new vertex count
		sliNumFillVertices = numVertices;
		sliFillRadius = radius;
		vertexPtr = vertices;

		// compute angle increment, and pre-compute sin and cos of that increment
		theta = ((degrees / 180.0) * PI) / (double)(sliNumFillVertices - 1);
		c = cos(theta);
		s = sin(theta);

		// first vertex position of semi circle
		x = radius;
		y = 0;

		// insert triangle fan center vertex, which is the extra (+1) vertex
		*vertexPtr++ = 0.0f;
		*vertexPtr++ = 0.0f;

		// compute new vertex positions
		for(i = 0; i < sliNumFillVertices; i ++)
		{
			*vertexPtr++ = (float)x;
			*vertexPtr++ = (float)y;

			// apply rotation matrix to current vertex position
			transform = x;
			x = c * x - s * y;
			y = s * transform + c * y;
		}

		// now pass the vertex data to OpenGL
		#ifndef USE_GLES
			glBindVertexArray(sliSemiCircleFillVAO);
		#endif
		glBindBuffer(GL_ARRAY_BUFFER, sliSemiCircleFillVBOs[0]);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(GLfloat) * (sliNumFillVertices + 1) * 3, vertices);		// + 1 for middle of triangle fan
	}

	// prepare our shader object
	shaderBind(sliBasicShader);
	shaderUniformMatrix4fv(sliBasicShader, "u_Modelview", 1, (float*)modelview);
	shaderUniform4f(sliBasicShader, "u_Color", (float)color -> x, (float)color -> y, (float)color -> z, (float)color -> w);

	// bind appropriate object state and render the object
	#ifndef USE_GLES
		glBindVertexArray(sliSemiCircleFillVAO);
	#else
		glBindBuffer(GL_ARRAY_BUFFER, sliSemiCircleFillVBOs[0]);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	#endif
	glDrawArrays(GL_TRIANGLE_FAN, 0, sliNumFillVertices + 1);				// + 1 for middle of triangle fan
}
