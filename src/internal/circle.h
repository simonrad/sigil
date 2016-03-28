#pragma once

#include "util/transform.h"

void sliCircleInit();
void sliCircleDestroy();

void sliCircleOutline(Mat4 *modelview, Vec4 *color, float radius, int numVertices);
void sliCircleFill(Mat4 *modelview, Vec4 *color, float radius, int numVertices);
