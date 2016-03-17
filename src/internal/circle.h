#pragma once

#include <glm/glm.hpp>

void sliCircleInit();
void sliCircleDestroy();

void sliCircleOutline(glm::mat4 &modelview, glm::vec4 &color, float radius, int numVertices);
void sliCircleFill(glm::mat4 &modelview, glm::vec4 &color, float radius, int numVertices);
