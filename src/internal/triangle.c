#include "triangle.h"
#include "shaders.h"

#include "../util/shader.h"

#include "config.h"

#ifdef USE_GLES
	#include <GLES2/gl2.h>
	#include <GLES2/gl2ext.h>
#else
	#include <GL/glew.h>
#endif

#include <stddef.h>

#ifdef USE_GLES
	static GLuint sliTriangleVAO = 0;
#endif
static GLuint sliTriangleVBOs[1] = {0};

void sliTriangleInit()
{
	GLfloat vertices[] = {0.0f, 0.5f,
						  -0.5f, -0.5f,
						  0.5f, -0.5f};

	// initialize our state object
	#ifndef USE_GLES
		glGenVertexArrays(1, &sliTriangleVAO);
		glBindVertexArray(sliTriangleVAO);
	#endif
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
	#ifndef USE_GLES
		glDeleteVertexArrays(1, &sliTriangleVAO);
	#endif
}

void sliTriangleOutline(Mat4 *modelview, Vec4 *color)
{
	// prepare our shader object
	shaderBind(sliBasicShader);
	shaderUniformMatrix4fv(sliBasicShader, "u_Modelview", 1, (float*)modelview);
	shaderUniform4f(sliBasicShader, "u_Color", color -> x, color -> y, color -> z, color -> w);

	// bind appropriate object state and render the object
	#ifndef USE_GLES
		glBindVertexArray(sliTriangleVAO);
	#endif
	glBindBuffer(GL_ARRAY_BUFFER, sliTriangleVBOs[0]);
	#ifdef USE_GLES
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, NULL);
	#endif
	glDrawArrays(GL_LINE_LOOP, 0, 3);
}

void sliTriangleFill(Mat4 *modelview, Vec4 *color)
{
	// prepare our shader object
	shaderBind(sliBasicShader);
	shaderUniformMatrix4fv(sliBasicShader, "u_Modelview", 1, (float*)modelview);
	shaderUniform4f(sliBasicShader, "u_Color", color -> x, color -> y, color -> z, color -> w);

	// bind appropriate object state and render the object
	#ifndef USE_GLES
		glBindVertexArray(sliTriangleVAO);
	#endif
	glBindBuffer(GL_ARRAY_BUFFER, sliTriangleVBOs[0]);
	#ifdef USE_GLES
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, (GLsizei)0, (char*)0);
	#endif
	glDrawArrays(GL_TRIANGLES, 0, 3);
}
