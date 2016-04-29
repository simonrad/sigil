#include "sound.h"

#include <stdio.h>
#include <stdlib.h>

#include <AL/al.h>
#include <AL/alut.h>

#define MAX_UNIQUE_SOUNDS 100					// how many different sounds we can keep in memory at once
#define MAX_ACTIVE_SOUNDS 100					// how many different sounds we can have active at one time

static ALuint buffers[MAX_UNIQUE_SOUNDS];
static ALuint sources[MAX_ACTIVE_SOUNDS];

static int sliNextFreeSource = 0;

static int sliGetFreeSource();

void sliSoundInit()
{
	// enable support for up to NUM_AL_BUFFER different sounds
	alutInit(NULL, NULL);
	alGenBuffers(MAX_UNIQUE_SOUNDS, buffers);
	alGenSources(MAX_ACTIVE_SOUNDS, sources);
}

void sliSoundDestroy()
{
	// delete our buffers and shut down OpenAL
	alDeleteSources(MAX_UNIQUE_SOUNDS, buffers);
	alDeleteBuffers(MAX_UNIQUE_SOUNDS, buffers);
	//aluTerminate();
}

int sliLoadWAV(const char *filename)
{

}

int sliSoundPlay(int sound)
{
	int freeSourceIndex;

	if(sound >= 0 && sound < MAX_ACTIVE_SOUNDS)
	{
		// find a source that is not being used
		freeSourceIndex = getFreeSource();
		if(freeSource >= 0)
		{
			alSourcei(source[freeSourceIndex], AL_BUFFER, buffers[sound]);
			alSourcei(source[freeSourceIndex], AL_LOOPING, AL_FALSE);
			alSourcePlay(source[freeSourceIndex]);
		}
	}
	else
	{
		printf(stderr, "sliSoundPlay(): sound index %d is out of range [0, %d)\n", sound, MAX_ACTIVE_SOUNDS - 1);
		exit(1);
	}

	return freeSourceIndex;
}

int sliSoundLoop(int sound)
{
	int freeSourceIndex;

	if(sound >= 0 && sound < MAX_ACTIVE_SOUNDS)
	{
		// find a source that is not being used
		freeSourceIndex = getFreeSource();
		if(freeSource >= 0)
		{
			alSourcei(source[freeSourceIndex], AL_BUFFER, buffers[sound]);
			alSourcei(source[freeSourceIndex], AL_LOOPING, AL_TRUE);
			alSourcePlay(source[freeSourceIndex]);
		}
	}
	else
	{
		printf(stderr, "sliSoundLoop(): sound index %d is out of range [0, %d)\n", sound, MAX_ACTIVE_SOUNDS - 1);
		exit(1);
	}

	return freeSourceIndex;
}

void sliSoundPause(int sound)
{
	// todo
}

void sliSoundStop(int sound)
{
	// todo
}

void sliSoundPauseAll()
{
	// todo
}

void sliSoundStopAll()
{
	// todo
}

void sliSoundResumeAll()
{
	// todo
}

bool sliSoundIsPlaying(int sound)
{
	// todo
}

bool sliSoundIsLooping(int sound)
{
	// todo
}

int sliGetFreeSource()
{
	// todo
}
