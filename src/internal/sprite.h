#pragma once

#include "../util/transform.h"

#include "config.h"

#ifdef USE_GLES
	#include <GLES2/gl2.h>
	#include <GLES2/gl2ext.h>
#else
	#include <GL/glew.h>
#endif

void sliSpriteInit();
void sliSpriteDestroy();
void sliSprite(Mat4 *modelview, Vec4 *color, GLuint texture, Vec2 *tiling, Vec2 *scroll);
