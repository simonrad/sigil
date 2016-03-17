#pragma once

#include <glm/glm.hpp>

void sliLineInit();
void sliLineDestroy();

void sliLine(glm::vec4 &color, double x1, double y1, double x2, double y2);
void sliLinesFlush();
