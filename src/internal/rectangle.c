#include "internal/rectangle.h"
#include "internal/shaders.h"

#include "util/shader.h"

#include <gl/glew.h>

static GLuint sliRectangleVAO = 0;
static GLuint sliRectangleVBOs[1] = {0};

void sliRectangleInit()
{
	GLfloat vertices[] = {-0.5, 0.5,
						  0.5, 0.5,
						  -0.5, -0.5,
						  0.5, -0.5};

	// initialize our state object
	glGenVertexArrays(1, &sliRectangleVAO);
	glBindVertexArray(sliRectangleVAO);
	glGenBuffers(1, sliRectangleVBOs);

	// configure vertex data
	glBindBuffer(GL_ARRAY_BUFFER, sliRectangleVBOs[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 8, vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(0);
}

void sliRectangleDestroy()
{
	glDeleteBuffers(1, sliRectangleVBOs);
	glDeleteVertexArrays(1, &sliRectangleVAO);
}

void sliRectangleOutline(Mat4 *modelview, Vec4 *color)
{
	// prepare our shader object
	shaderBind(sliBasicShader);
	shaderUniformMatrix4fv(sliBasicShader, "u_Modelview", 1, (float*)modelview);
	shaderUniform4f(sliBasicShader, "u_Color", color -> x, color -> y, color -> z, color -> w);

	// bind appropriate object state and render the object
	glBindVertexArray(sliRectangleVAO);
	glDrawArrays(GL_LINE_LOOP, 0, 4);
}

void sliRectangleFill(Mat4 *modelview, Vec4 *color)
{
	// prepare our shader object
	shaderBind(sliBasicShader);
	shaderUniformMatrix4fv(sliBasicShader, "u_Modelview", 1, (float*)modelview);
	shaderUniform4f(sliBasicShader, "u_Color", color -> x, color -> y, color -> z, color -> w);

	// bind appropriate object state and render the object
	glBindVertexArray(sliRectangleVAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}
