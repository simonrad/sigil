#pragma once

#include <gl/glew.h>

void imageLoaderInit();
void imageLoaderKill();

GLuint loadImage(const char *filename);
