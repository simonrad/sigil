#include "sprite.h"
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
	static GLuint sliSpriteVAO = 0;
#endif
static GLuint sliSpriteVBOs[2] = {0, 0};

void sliSpriteInit()
{
	GLfloat vertices[] = {-0.5f, 0.5f,
						  0.5f, 0.5f,
						  -0.5f, -0.5f,
						  0.5f, -0.5f};

	GLfloat texCoords[] = {0.0f, 1.0f,
						   1.0f, 1.0f,
						   0.0f, 0.0f,
						   1.0f, 0.0f};

	// initialize our state object
	#ifndef USE_GLES
		glGenVertexArrays(1, &sliSpriteVAO);
		glBindVertexArray(sliSpriteVAO);
	#endif
	glGenBuffers(2, sliSpriteVBOs);

	// configure vertex data
	glBindBuffer(GL_ARRAY_BUFFER, sliSpriteVBOs[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 8, vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(0);

	// configure texture coord data
	glBindBuffer(GL_ARRAY_BUFFER, sliSpriteVBOs[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 8, texCoords, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(1);
}

void sliSpriteDestroy()
{
	glDeleteBuffers(2, sliSpriteVBOs);
	#ifndef USE_GLES
		glDeleteVertexArrays(1, &sliSpriteVAO);
	#endif
}

void sliSprite(Mat4 *modelview, Vec4 *color, GLuint texture, Vec2 *tiling, Vec2 *scroll)
{
	// prepare our shader object
	shaderBind(sliTextureShader);
	shaderUniformMatrix4fv(sliTextureShader, "u_Modelview", 1, (float*)modelview);
	shaderUniform4f(sliTextureShader, "u_Color", (float)color -> x, (float)color -> y, (float)color -> z, (float)color -> w);
	shaderUniform2f(sliTextureShader, "u_Tiling", (float)tiling -> x, (float)tiling -> y);
	shaderUniform2f(sliTextureShader, "u_Scroll", (float)scroll -> x, (float)scroll -> y);

	// bind appropriate object state and render the object
	#ifndef USE_GLES
		glBindVertexArray(sliSpriteVAO);
	#else
		glBindBuffer(GL_ARRAY_BUFFER, sliSpriteVBOs[0]);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, NULL);
		glBindBuffer(GL_ARRAY_BUFFER, sliSpriteVBOs[1]);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, NULL);
	#endif
	glBindTexture(GL_TEXTURE_2D, texture);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}
