#include "rectangle.h"
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

#ifndef USE_GLES
	static GLuint sliRectangleFillVAO = 0;
#endif
static GLuint sliRectangleFillVBOs[1] = {0};

#ifndef USE_GLES
	static GLuint sliRectangleOutlineVAO = 0;
#endif
static GLuint sliRectangleOutlineVBOs[1] = {0};

void sliRectangleInit()
{
	GLfloat outlineVertices[] = {-0.5, 0.5,
								-0.5, -0.5,
								0.5, -0.5,
								0.5, 0.5};

	GLfloat fillVertices[] = {-0.5, -0.5,
							  -0.5, 0.5,
							  0.5, -0.5,
							  0.5, 0.5};

	// intialize our outline state object
	#ifndef USE_GLES
		glGenVertexArrays(1, &sliRectangleOutlineVAO);
		glBindVertexArray(sliRectangleOutlineVAO);
	#endif
	glGenBuffers(1, sliRectangleOutlineVBOs);

	// configure outline vertex data
	glBindBuffer(GL_ARRAY_BUFFER, sliRectangleOutlineVBOs[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 8, outlineVertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(0);

	// initialize our fill state object
	#ifndef USE_GLES
		glGenVertexArrays(1, &sliRectangleFillVAO);
		glBindVertexArray(sliRectangleFillVAO);
	#endif
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
	#ifndef USE_GLES
		glDeleteVertexArrays(1, &sliRectangleOutlineVAO);
	#endif

	glDeleteBuffers(1, sliRectangleFillVBOs);
	#ifndef USE_GLES
		glDeleteVertexArrays(1, &sliRectangleFillVAO);
	#endif
}

void sliRectangleOutline(Mat4 *modelview, Vec4 *color)
{
	// prepare our shader object
	shaderBind(sliBasicShader);
	shaderUniformMatrix4fv(sliBasicShader, "u_Modelview", 1, (float*)modelview);
	shaderUniform4f(sliBasicShader, "u_Color", color -> x, color -> y, color -> z, color -> w);

	// bind appropriate object state and render the object
	#ifndef USE_GLES
		glBindVertexArray(sliRectangleOutlineVAO);
	#endif
	glBindBuffer(GL_ARRAY_BUFFER, sliRectangleOutlineVBOs[0]);
	#ifdef USE_GLES
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, NULL);
	#endif
	glDrawArrays(GL_LINE_LOOP, 0, 4);
}

void sliRectangleFill(Mat4 *modelview, Vec4 *color)
{
	// prepare our shader object
	shaderBind(sliBasicShader);
	shaderUniformMatrix4fv(sliBasicShader, "u_Modelview", 1, (float*)modelview);
	shaderUniform4f(sliBasicShader, "u_Color", color -> x, color -> y, color -> z, color -> w);

	// bind appropriate object state and render the object
	#ifndef USE_GLES
		glBindVertexArray(sliRectangleFillVAO);
	#endif
	glBindBuffer(GL_ARRAY_BUFFER, sliRectangleFillVBOs[0]);
	#ifdef USE_GLES
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, NULL);
	#endif
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}
