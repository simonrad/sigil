#include "triangle.h"
#include "shaders.h"

#include "../util/shader.h"

#include <GL/glew.h>

static GLuint sliTriangleVAO = 0;
static GLuint sliTriangleVBOs[1] = {0};

void sliTriangleInit()
{
	GLfloat vertices[] = {0.0, 0.5,
						  -0.5, -0.5,
						  0.5, -0.5};

	// initialize our state object
	glGenVertexArrays(1, &sliTriangleVAO);
	glBindVertexArray(sliTriangleVAO);
	glGenBuffers(1, sliTriangleVBOs);

	// configure vertex data
	glBindBuffer(GL_ARRAY_BUFFER, sliTriangleVBOs[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6, vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(0);
}

void sliTriangleDestroy()
{
	glDeleteBuffers(1, sliTriangleVBOs);
	glDeleteVertexArrays(1, &sliTriangleVAO);
}

void sliTriangleOutline(Mat4 *modelview, Vec4 *color)
{
	// prepare our shader object
	shaderBind(sliBasicShader);
	shaderUniformMatrix4fv(sliBasicShader, "u_Modelview", 1, (float*)modelview);
	shaderUniform4f(sliBasicShader, "u_Color", color -> x, color -> y, color -> z, color -> w);

	// bind appropriate object state and render the object
	glBindVertexArray(sliTriangleVAO);
	glDrawArrays(GL_LINE_LOOP, 0, 3);
}

void sliTriangleFill(Mat4 *modelview, Vec4 *color)
{
	// prepare our shader object
	shaderBind(sliBasicShader);
	shaderUniformMatrix4fv(sliBasicShader, "u_Modelview", 1, (float*)modelview);
	shaderUniform4f(sliBasicShader, "u_Color", color -> x, color -> y, color -> z, color -> w);

	// bind appropriate object state and render the object
	glBindVertexArray(sliTriangleVAO);
	glDrawArrays(GL_TRIANGLES, 0, 3);
}
