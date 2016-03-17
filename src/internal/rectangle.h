#pragma once

#include <glm/glm.hpp>

void sliRectangleInit();
void sliRectangleDestroy();

void sliRectangleOutline(glm::mat4 &modelview, glm::vec4 &color);
void sliRectangleFill(glm::mat4 &modelview, glm::vec4 &color);
