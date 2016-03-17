#include "internal/circle.h"
#include "internal/shaders.h"

#include "assets/shader.h"

#include <gl/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
using namespace glm;

#define PI 3.1415926

#define MIN_VERTICES 3
#define MAX_VERTICES 100

static GLuint sliCircleOutlineVAO = 0;
static GLuint sliCircleOutlineVBOs[1] = {0};

static GLuint sliCircleFillVAO = 0;
static GLuint sliCircleFillVBOs[1] = {0};

static int sliNumOutlineVertices = 0;
static int sliNumFillVertices = 0;

void sliCircleInit()
{
	// initialize our outline state object
	glGenVertexArrays(1, &sliCircleOutlineVAO);
	glBindVertexArray(sliCircleOutlineVAO);
	glGenBuffers(1, sliCircleOutlineVBOs);

	// configure outline vertex data
	glBindBuffer(GL_ARRAY_BUFFER, sliCircleOutlineVBOs[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * MAX_VERTICES * 3, NULL, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(0);

	// initialize our fill state object
	glGenVertexArrays(1, &sliCircleFillVAO);
	glBindVertexArray(sliCircleFillVAO);
	glGenBuffers(1, sliCircleFillVBOs);

	// configure fill vertex data
	glBindBuffer(GL_ARRAY_BUFFER, sliCircleFillVBOs[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * (MAX_VERTICES + 2) * 3, NULL, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(0);
}

void sliCircleDestroy()
{

}

void sliCircleOutline(mat4 &modelview, vec4 &color, float radius, int numVertices)
{
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
	if(numVertices != sliNumOutlineVertices)
	{
		// assign new vertex count
		sliNumOutlineVertices = numVertices;

		// allocate space for new circle vertices
		vertices = new GLfloat[sliNumOutlineVertices * 3];

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
		glBindVertexArray(sliCircleOutlineVAO);
		glBindBuffer(GL_ARRAY_BUFFER, sliCircleOutlineVBOs[0]);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(GLfloat) * sliNumOutlineVertices * 3, vertices);

		// done with vertex data
		delete[] vertices;
	}

	// prepare our shader object
	sliBasicShader -> bind();
	sliBasicShader -> uniformMatrix4fv("u_Modelview", 1, value_ptr(modelview));
	sliBasicShader -> uniformVec4("u_Color", color);

	// bind appropriate object state and render the object
	glBindVertexArray(sliCircleOutlineVAO);
	glDrawArrays(GL_LINE_LOOP, 0, sliNumOutlineVertices);
}

void sliCircleFill(mat4 &modelview, vec4 &color, float radius, int numVertices)
{
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
	if(numVertices != sliNumFillVertices)
	{
		// assign new vertex count
		sliNumFillVertices = numVertices;

		// allocate space for new circle vertices (we use +2 for the center vertex and for the last vertex of
		// the triangle fan, since it doesn't loop back and close itself on its own)
		vertices = new GLfloat[(sliNumFillVertices + 2) * 3];

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
		glBindVertexArray(sliCircleFillVAO);
		glBindBuffer(GL_ARRAY_BUFFER, sliCircleFillVBOs[0]);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(GLfloat) * (sliNumFillVertices + 2) * 3, vertices);

		// done with vertex data
		delete[] vertices;
	}

	// prepare our shader object
	sliBasicShader -> bind();
	sliBasicShader -> uniformMatrix4fv("u_Modelview", 1, value_ptr(modelview));
	sliBasicShader -> uniformVec4("u_Color", color);

	// bind appropriate object state and render the object
	glBindVertexArray(sliCircleFillVAO);
	glDrawArrays(GL_TRIANGLE_FAN, 0, sliNumFillVertices + 2);
}
