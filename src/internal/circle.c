#include "circle.h"
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
	static GLuint sliCircleOutlineVAO = 0;
#endif
static GLuint sliCircleOutlineVBOs[1] = {0};

#ifndef USE_GLES
	static GLuint sliCircleFillVAO = 0;
#endif
static GLuint sliCircleFillVBOs[1] = {0};

static float sliOutlineRadius = 0;
static float sliFillRadius = 0;

static int sliNumOutlineVertices = 0;
static int sliNumFillVertices = 0;

void sliCircleInit()
{
	// initialize our outline state object
	#ifndef USE_GLES
		glGenVertexArrays(1, &sliCircleOutlineVAO);
		glBindVertexArray(sliCircleOutlineVAO);
	#endif
	glGenBuffers(1, sliCircleOutlineVBOs);

	// configure outline vertex data
	glBindBuffer(GL_ARRAY_BUFFER, sliCircleOutlineVBOs[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * MAX_VERTICES * 3, NULL, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(0);

	// initialize our fill state object
	#ifndef USE_GLES
		glGenVertexArrays(1, &sliCircleFillVAO);
		glBindVertexArray(sliCircleFillVAO);
	#endif
	glGenBuffers(1, sliCircleFillVBOs);

	// configure fill vertex data
	glBindBuffer(GL_ARRAY_BUFFER, sliCircleFillVBOs[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * (MAX_VERTICES + 2) * 3, NULL, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(0);
}

void sliCircleDestroy()
{
	glDeleteBuffers(1, sliCircleOutlineVBOs);
	#ifndef USE_GLES
		glDeleteVertexArrays(1, &sliCircleOutlineVAO);
	#endif

	glDeleteBuffers(1, sliCircleFillVBOs);
	#ifndef USE_GLES
		glDeleteVertexArrays(1, &sliCircleFillVAO);
	#endif
}

void sliCircleOutline(Mat4 *modelview, Vec4 *color, float radius, int numVertices)
{
	const float EPS = 0.00001;

	GLfloat vertices[MAX_VERTICES * 3];
	float theta;
	float transform;
	float c, s;
	float x, y;
	int i;

	// make sure the number of vertices is something reasonable
	if(numVertices < MIN_VERTICES) numVertices = MIN_VERTICES;
	if(numVertices > MAX_VERTICES) numVertices = MAX_VERTICES;

	// if the number of vertices specified is different from what we've allocated, prepare to re-compute them
	if(numVertices != sliNumOutlineVertices || fabs(radius - sliOutlineRadius) > EPS)
	{
		// assign new vertex count and radius
		sliNumOutlineVertices = numVertices;
		sliOutlineRadius = radius;

		// compute angle increment, and pre-compute sin and cos of that increment
		theta = 2 * (PI / (float)sliNumOutlineVertices);
		c = cosf(theta);
		s = sinf(theta);

		// first vertex position of line loop
		x = radius;
		y = 0;

		// compute new vertex positions
		for(i = 0; i < sliNumOutlineVertices * 3; i += 3)
		{
			vertices[i + 0] = x;
			vertices[i + 1] = y;
			vertices[i + 2] = 0.0;

			// apply rotation matrix to current vertex position
			transform = x;
			x = c * x - s * y;
			y = s * transform + c * y;
		}

		// now pass the vertex data to OpenGL
		#ifndef USE_GLES
			glBindVertexArray(sliCircleOutlineVAO);
		#endif
		glBindBuffer(GL_ARRAY_BUFFER, sliCircleOutlineVBOs[0]);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(GLfloat) * sliNumOutlineVertices * 3, vertices);
	}

	// prepare our shader object
	shaderBind(sliBasicShader);
	shaderUniformMatrix4fv(sliBasicShader, "u_Modelview", 1, (float*)modelview);
	shaderUniform4f(sliBasicShader, "u_Color", color -> x, color -> y, color -> z, color -> w);

	// bind appropriate object state and render the object
	#ifndef USE_GLES
		glBindVertexArray(sliCircleOutlineVAO);
	#else
		glBindBuffer(GL_ARRAY_BUFFER, sliCircleOutlineVBOs[0]);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	#endif
	glDrawArrays(GL_LINE_LOOP, 0, sliNumOutlineVertices);
}

void sliCircleFill(Mat4 *modelview, Vec4 *color, float radius, int numVertices)
{
	const float EPS = 0.00001;

	GLfloat *vertices;
	float theta;
	float transform;
	float c, s;
	float x, y;
	int i;

	// make sure the number of vertices is something reasonable
	if(numVertices < MIN_VERTICES) numVertices = MIN_VERTICES;
	if(numVertices > MAX_VERTICES) numVertices = MAX_VERTICES;

	// if the number of vertices specified is different from what we've allocated, prepare to re-compute them
	if(numVertices != sliNumFillVertices || fabs(radius - sliFillRadius) > EPS)
	{
		// assign new vertex count
		sliNumFillVertices = numVertices;
		sliFillRadius = radius;

		// allocate space for new circle vertices (we use +2 for the center vertex and for the last vertex of
		// the triangle fan, since it doesn't loop back and close itself on its own)
		vertices = (GLfloat*)malloc(sizeof(GLfloat) * (sliNumFillVertices + 2) * 3);

		// compute angle increment, and pre-compute sin and cos of that increment
		theta = 2 * (PI / (float)sliNumFillVertices);
		c = cosf(theta);
		s = sinf(theta);

		// first vertex position of line loop
		x = radius;
		y = 0;

		// insert triangle fan center vertex
		vertices[0] = 0.0;
		vertices[1] = 0.0;
		vertices[2] = 0.0;

		// compute new vertex positions
		for(i = 3; i < (sliNumFillVertices + 2) * 3; i += 3)
		{
			vertices[i + 0] = x;
			vertices[i + 1] = y;
			vertices[i + 2] = 0.0;

			// apply rotation matrix to current vertex position
			transform = x;
			x = c * x - s * y;
			y = s * transform + c * y;
		}

		// now pass the vertex data to OpenGL
		#ifndef USE_GLES
			glBindVertexArray(sliCircleFillVAO);
		#endif
		glBindBuffer(GL_ARRAY_BUFFER, sliCircleFillVBOs[0]);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(GLfloat) * (sliNumFillVertices + 2) * 3, vertices);

		// done with vertex data
		free(vertices);
	}

	// prepare our shader object
	shaderBind(sliBasicShader);
	shaderUniformMatrix4fv(sliBasicShader, "u_Modelview", 1, (float*)modelview);
	shaderUniform4f(sliBasicShader, "u_Color", color -> x, color -> y, color -> z, color -> w);

	// bind appropriate object state and render the object
	#ifndef USE_GLES
		glBindVertexArray(sliCircleFillVAO);
	#else
		glBindBuffer(GL_ARRAY_BUFFER, sliCircleFillVBOs[0]);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	#endif
	glDrawArrays(GL_TRIANGLE_FAN, 0, sliNumFillVertices + 2);
}
