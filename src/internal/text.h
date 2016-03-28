#pragma once

#include "util/transform.h"

void sliTextInit();
void sliTextDestroy();

void sliTextAlign(int fontAlign);
double sliTextWidth(const char *text);
double sliTextHeight(const char *text);
void sliText(Mat4 *modelview, Vec4 *color, const char *text);
void sliTextFlush(Mat4 *modelview, Vec4 *color);

void sliFont(const char *fontFilename, int fontSize);
void sliFontSize(int fontSize);
