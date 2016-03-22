#pragma once

/*
Simple Shape Drawing Library (SSDL), for Windows.
Geoff Nagy
*/

// initialization commands

void slWindow(int width, int height, const char *title);
void slClose();

// simple input

bool slGetKey(int key);

bool slGetMouseButton(int button);
void slGetMousePos(int &posX, int &posY);

// rendering commands

void slRender();

// color and drawing control

void slSetBackColor(double red, double green, double blue);
void slSetForeColor(double red, double green, double blue, double alpha);

// blending control

void slAdditiveBlend(bool additiveBlend);

// transformations

void slPush();
void slPop();

void slTranslate(double x, double y);
void slRotate(double degrees);
void slScale(double x, double y);

// simple shape commands

void slTriangleFill(double x, double y, double width, double height);
void slTriangleOutline(double x, double y, double width, double height);

void slRectangleFill(double x, double y, double width, double height);
void slRectangleOutline(double x, double y, double width, double height);

void slCircleFill(double x, double y, double radius, int numVertices);
void slCircleOutline(double x, double y, double radius, int numVertices);

void slPoint(double x, double y);

void slLine(double x1, double y1, double x2, double y2);

void slSprite(char *textureFilename, double x, double y, double width, double height);
void slSprite(char *textureFilename, double x, double y, double width, double height, double tilingX, double tilingY);
void slSprite(char *textureFilename, double x, double y, double width, double height, double tilingX, double tilingY, double scrollX, double scrollY);

// text commands

void slTextAlign(int fontAlign);
double slTextWidth(const char *text);
double slTextHeight(const char *text);
void slText(double x, double y, const char *text);
void slFont(const char *fontFilename);
void slFontSize(int fontSize);
