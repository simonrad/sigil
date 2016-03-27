#include "internal/rectangle.h"
#include "internal/shaders.h"

#include "assets/shader.h"

#include <gl/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
using namespace glm;

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

void sliRectangleOutline(mat4 &modelview, vec4 &color)
{
	// prepare our shader object
	sliBasicShader -> bind();
	sliBasicShader -> uniformMatrix4fv("u_Modelview", 1, value_ptr(modelview));
	sliBasicShader -> uniformVec4("u_Color", color);

	// bind appropriate object state and render the object
	glBindVertexArray(sliRectangleVAO);
	glDrawArrays(GL_LINE_LOOP, 0, 4);
}

void sliRectangleFill(mat4 &modelview, vec4 &color)
{
	// prepare our shader object
	sliBasicShader -> bind();
	sliBasicShader -> uniformMatrix4fv("u_Modelview", 1, value_ptr(modelview));
	sliBasicShader -> uniformVec4("u_Color", color);

	// bind appropriate object state and render the object
	glBindVertexArray(sliRectangleVAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}
