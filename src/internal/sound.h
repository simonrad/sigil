#pragma once

#include <AL/al.h>

void sliSoundInit();
void sliSoundDestroy();

int sliLoadWAV(const char *filename);
int sliSoundPlay(int sound);
int sliSoundLoop(int sound);
void sliSoundPause(int sound);
void sliSoundStop(int sound);
void sliSoundPauseAll();
void sliSoundStopAll();
void sliSoundResumeAll();
int sliSoundPlaying(int sound);
int sliSoundLooping(int sound);
