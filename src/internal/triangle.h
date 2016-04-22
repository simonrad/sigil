#pragma once

#include "../util/transform.h"

void sliTriangleInit();
void sliTriangleDestroy();

void sliTriangleOutline(Mat4 *modelview, Vec4 *color);
void sliTriangleFill(Mat4 *modelview, Vec4 *color);
