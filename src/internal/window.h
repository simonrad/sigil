#pragma once

void sliOpenWindow(
    int width,
    int height,
    const char *title,
    int fullScreen,
    int *frameWidth,
    int *frameHeight
);

void sliShowCursor(int showCursor);
void sliCloseWindow();
int sliIsWindowOpen();

int sliShouldClose();

int sliGetKey(int key);
int sliGetMouseButton(int button);
void sliGetMousePos(int *posX, int *posY);

double sliGetTime();
void sliPollAndSwap();
