#include "internal/sprite.h"
#include "internal/shaders.h"

#include "assets/shader.h"

#include <gl/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
using namespace glm;

static GLuint sliSpriteVAO = 0;
static GLuint sliSpriteVBOs[2] = {0, 0};

void sliSpriteInit()
{
	GLfloat vertices[] = {-0.5, 0.5,
						  0.5, 0.5,
						  -0.5, -0.5,
						  0.5, -0.5};

	GLfloat texCoords[] = {0.0, 1.0,
						   1.0, 1.0,
						   0.0, 0.0,
						   1.0, 0.0};

	// initialize our state object
	glGenVertexArrays(1, &sliSpriteVAO);
	glBindVertexArray(sliSpriteVAO);
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
	glDeleteVertexArrays(1, &sliSpriteVAO);
}

void sliSprite(mat4 &modelview, vec4 &color, GLuint texture, vec2 &tiling, vec2 &scroll)
{
	// prepare our shader object
	sliTextureShader -> bind();
	sliTextureShader -> uniformMatrix4fv("u_Modelview", 1, value_ptr(modelview));
	sliTextureShader -> uniformVec4("u_Color", color);
	sliTextureShader -> uniformVec2("u_Tiling", tiling);
	sliTextureShader -> uniformVec2("u_Scroll", scroll);

	// bind appropriate object state and render the object
	glBindVertexArray(sliSpriteVAO);
	glBindTexture(GL_TEXTURE_2D, texture);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}
