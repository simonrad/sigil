#pragma once

#include <glm/glm.hpp>

void sliTriangleInit();
void sliTriangleDestroy();

void sliTriangleOutline(glm::mat4 &modelview, glm::vec4 &color);
void sliTriangleFill(glm::mat4 &modelview, glm::vec4 &color);
