#include "internal/triangle.h"
#include "internal/shaders.h"

#include "assets/shader.h"

#include <gl/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
using namespace glm;

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

void sliTriangleOutline(mat4 &modelview, vec4 &color)
{
	// prepare our shader object
	sliBasicShader -> bind();
	sliBasicShader -> uniformMatrix4fv("u_Modelview", 1, value_ptr(modelview));
	sliBasicShader -> uniformVec4("u_Color", color);

	// bind appropriate object state and render the object
	glBindVertexArray(sliTriangleVAO);
	glDrawArrays(GL_LINE_LOOP, 0, 3);
}

void sliTriangleFill(mat4 &modelview, vec4 &color)
{
	// prepare our shader object
	sliBasicShader -> bind();
	sliBasicShader -> uniformMatrix4fv("u_Modelview", 1, value_ptr(modelview));
	sliBasicShader -> uniformVec4("u_Color", color);

	// bind appropriate object state and render the object
	glBindVertexArray(sliTriangleVAO);
	glDrawArrays(GL_TRIANGLES, 0, 3);
}
