#pragma once

#include "../util/transform.h"

void sliLineInit();
void sliLineDestroy();

void sliLine(Vec4 *color, double x1, double y1, double x2, double y2);
void sliLinesFlush();
