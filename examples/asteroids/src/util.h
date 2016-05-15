#pragma once

#include "glm/glm.hpp"

bool rayIntersectsSphere(glm::vec2 point1, glm::vec2 point2, glm::vec2 sphere, float radius, glm::vec2 &intersect);
