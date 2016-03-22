#pragma once

#include <glm/glm.hpp>

void sliTextInit();
void sliTextAlign(int fontAlign);
double sliTextWidth(const char *text);
double sliTextHeight(const char *text);
void sliText(glm::mat4 &modelview, glm::vec4 &color, const char *text);
void sliTextFlush(glm::mat4 &modelview, glm::vec4 &color);

void sliFont(int fontID);
void sliFont(const char *fontFilename, int fontSize);
//void sliFontSize(int fontSize);
