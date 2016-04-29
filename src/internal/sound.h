#pragma once

#include <AL/al.h>

void sliSoundInit();
void sliSoundDestroy();

ALuint sliLoadWAV(const char *filename);
ALuint sliSoundPlay(ALuint sound);
ALuint sliSoundLoop(ALuint sound);
void sliSoundPause(ALuint sound);
void sliSoundStop(ALuint sound);
void sliSoundPauseAll();
void sliSoundStopAll();
void sliSoundResumeAll();
bool sliIsSoundPlaying(ALuint sound);
bool sliIsSoundLooping(ALuint sound);
