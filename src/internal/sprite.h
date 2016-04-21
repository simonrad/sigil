#pragma once

#include "util/transform.h"

#include "gl/glew.h"

void sliSpriteInit();
void sliSpriteDestroy();
void sliSprite(Mat4 *modelview, Vec4 *color, GLuint texture, Vec2 *tiling, Vec2 *scroll);
