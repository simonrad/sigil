#pragma once

#include <glm/glm.hpp>

void sliPointInit();
void sliPointDestroy();

void sliPoint(glm::mat4 &modelview, glm::vec4 &color);
void sliPointsFlush();
