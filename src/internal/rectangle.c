#include "rectangle.h"
#include "shaders.h"

#include "../util/shader.h"

#include <GL/glew.h>

static GLuint sliRectangleFillVAO = 0;
static GLuint sliRectangleFillVBOs[1] = {0};

static GLuint sliRectangleOutlineVAO = 0;
static GLuint sliRectangleOutlineVBOs[1] = {0};

void sliRectangleInit()
{
	GLfloat outlineVerices[] = {-0.5, 0.5,
								-0.5, -0.5,
								0.5, -0.5,
								0.5, 0.5};

	GLfloat fillVertices[] = {-0.5, 0.5,
							  0.5, 0.5,
							  -0.5, -0.5,
							  0.5, -0.5};

	// intialize our outline state object
	glGenVertexArrays(1, &sliRectangleOutlineVAO);
	glBindVertexArray(sliRectangleOutlineVAO);
	glGenBuffers(1, sliRectangleOutlineVBOs);

	// configure outline vertex data
	glBindBuffer(GL_ARRAY_BUFFER, sliRectangleOutlineVBOs[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 8, outlineVerices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(0);

	// initialize our fill state object
	glGenVertexArrays(1, &sliRectangleFillVAO);
	glBindVertexArray(sliRectangleFillVAO);
	glGenBuffers(1, sliRectangleFillVBOs);

	// configure fill vertex data
	glBindBuffer(GL_ARRAY_BUFFER, sliRectangleFillVBOs[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 8, fillVertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(0);
}

void sliRectangleDestroy()
{
	glDeleteBuffers(1, sliRectangleOutlineVBOs);
	glDeleteVertexArrays(1, &sliRectangleOutlineVAO);

	glDeleteBuffers(1, sliRectangleFillVBOs);
	glDeleteVertexArrays(1, &sliRectangleFillVAO);
}

void sliRectangleOutline(Mat4 *modelview, Vec4 *color)
{
	// prepare our shader object
	shaderBind(sliBasicShader);
	shaderUniformMatrix4fv(sliBasicShader, "u_Modelview", 1, (float*)modelview);
	shaderUniform4f(sliBasicShader, "u_Color", color -> x, color -> y, color -> z, color -> w);

	// bind appropriate object state and render the object
	glBindVertexArray(sliRectangleOutlineVAO);
	glDrawArrays(GL_LINE_LOOP, 0, 4);
}

void sliRectangleFill(Mat4 *modelview, Vec4 *color)
{
	// prepare our shader object
	shaderBind(sliBasicShader);
	shaderUniformMatrix4fv(sliBasicShader, "u_Modelview", 1, (float*)modelview);
	shaderUniform4f(sliBasicShader, "u_Color", color -> x, color -> y, color -> z, color -> w);

	// bind appropriate object state and render the object
	glBindVertexArray(sliRectangleFillVAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}
