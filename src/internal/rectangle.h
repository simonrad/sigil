#pragma once

#include "../util/transform.h"

void sliRectangleInit();
void sliRectangleDestroy();

void sliRectangleOutline(Mat4 *modelview, Vec4 *color);
void sliRectangleFill(Mat4 *modelview, Vec4 *color);
