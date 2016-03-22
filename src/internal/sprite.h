#pragma once

#include <gl/glew.h>

#include <glm/glm.hpp>

void sliSpriteInit();
void sliSpriteDestroy();
void sliSprite(glm::mat4 &modelview, glm::vec4 &color, GLuint texture, glm::vec2 &tiling, glm::vec2 &scroll);
