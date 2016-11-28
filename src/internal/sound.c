#include "sound.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include <al.h>
#include <alc.h>

#define MAX_BUFFERS 100						// how many different sounds we can keep in memory at once
#define MAX_SOURCES 100						// how many different sounds we can have active at one time

// SIGIL owes thanks to the following websites for their usefulness:
//	- for loading wav files without ALUT: http://www.dunsanyinteractive.com/blogs/oliver/?p=72
//		- a few changes had to be made to port this to pure C, including the use of portable types (uint32_t, etc.) instead of long, short, etc.
//  - for initializing/deinitializing OpenAL without ALUT: http://kcat.strangesoft.net/openal-tutorial.html

static ALCdevice *sliDevice;
static ALCcontext *sliContext;

static int sliNextFreeBuffer = 0;
static ALuint buffers[MAX_BUFFERS];

static int sliNextFreeSource = 0;
static ALuint sources[MAX_SOURCES];

static void sliLoadWaveFormat(const char *filename, ALuint buffer);
static int sliGetFreeSource(void);
static int sliIsIndexValid(int sourceIndex);

/*
 * (From http://www.dunsanyinteractive.com/blogs/oliver/?p=72)
 * Struct that holds the RIFF data of the Wave file.
 * The RIFF data is the meta data information that holds,
 * the ID, size and format of the wave file
 */
struct RIFF_Header
{
	int8_t chunkID[4];
	int32_t chunkSize;			// size not including chunkSize or chunkID; we use int32_t for guaranteed type sizes (original code uses long)
	int8_t format[4];
};

/*
 * (From http://www.dunsanyinteractive.com/blogs/oliver/?p=72)
 * Struct to hold fmt subchunk data for WAVE files.
 */
struct WAVE_Format
{
	int8_t subChunkID[4];
	int32_t subChunkSize;
	int16_t audioFormat;
	int16_t numChannels;
	int32_t sampleRate;
	int32_t byteRate;
	int16_t blockAlign;
	int16_t bitsPerSample;
};

/*
 * (From http://www.dunsanyinteractive.com/blogs/oliver/?p=72)
 * Struct to hold the data of the wave file
 */
struct WAVE_Data
{
	int8_t subChunkID[4]; 		// should contain the word data
	int32_t subChunk2Size; 		// stores the size of the data block
};

// special thanks to http://kcat.strangesoft.net/openal-tutorial.html for this
void sliSoundInit(void)
{
	// open our audio device
    sliDevice = alcOpenDevice(NULL);
    if(!sliDevice)
    {
        fprintf(stderr, "sliSoundInit() could not open audio device\n");
        exit(1);
    }

    // create an OpenAL context
    sliContext = alcCreateContext(sliDevice, NULL);
    alcMakeContextCurrent(sliContext);
    if(!sliContext)
    {
        fprintf(stderr, "sliSoundInit() could not create audio context\n");
        exit(1);
    }

	// enable support for up to MAX_BUFFERS different sounds
	alGenBuffers(MAX_BUFFERS, buffers);
	alGenSources(MAX_SOURCES, sources);
}

void sliSoundDestroy()
{
	// delete our buffers and sources
	alDeleteSources(MAX_BUFFERS, buffers);
	alDeleteBuffers(MAX_BUFFERS, buffers);

	// shut down our context and close the audio device (thanks http://kcat.strangesoft.net/openal-tutorial.html)
	alcMakeContextCurrent(NULL);
    alcDestroyContext(sliContext);
    alcCloseDevice(sliDevice);
}

int sliLoadWAV(const char *filename)
{
	if(sliNextFreeBuffer < MAX_BUFFERS)
	{
		sliLoadWaveFormat(filename, buffers[sliNextFreeBuffer++]);
	}
	else
	{
		fprintf(stderr, "sliLoadWAV() has already reached the maximum number of OpenAL buffers, %d\n", MAX_BUFFERS);
		fprintf(stderr, "\t(Please email Geoff at geoff@libsigil.com and tell him that he stupidly thought %d buffers would be enough!)\n", MAX_BUFFERS);
		exit(1);
	}

	return sliNextFreeBuffer - 1;
}

int sliSoundPlay(int sound)
{
	int freeSourceIndex;

	if(sound >= 0 && sound < MAX_SOURCES)
	{
		// find a source that is not being used
		freeSourceIndex = sliGetFreeSource();
		if(freeSourceIndex >= 0)
		{
			alSourcei(sources[freeSourceIndex], AL_BUFFER, buffers[sound]);
			alSourcei(sources[freeSourceIndex], AL_LOOPING, AL_FALSE);
			alSourcePlay(sources[freeSourceIndex]);
		}
	}
	else
	{
		fprintf(stderr, "sliSoundPlay(): sound index %d is out of range [0, %d)\n", sound, (MAX_SOURCES - 1));
		exit(1);
	}

	return freeSourceIndex;
}

int sliSoundLoop(int sound)
{
	int freeSourceIndex;

	if(sound >= 0 && sound < MAX_SOURCES)
	{
		// find a source that is not being used
		freeSourceIndex = sliGetFreeSource();
		if(freeSourceIndex >= 0)
		{
			alSourcei(sources[freeSourceIndex], AL_BUFFER, buffers[sound]);
			alSourcei(sources[freeSourceIndex], AL_LOOPING, AL_TRUE);
			alSourcePlay(sources[freeSourceIndex]);
		}
	}
	else
	{
		fprintf(stderr, "sliSoundLoop(): sound index %d is out of range [0, %d)\n", sound, MAX_SOURCES - 1);
		exit(1);
	}

	return freeSourceIndex;
}

void sliSoundPause(int sound)
{
	if(sliIsIndexValid(sound))
	{
		alSourcePause(sources[sound]);
	}
	else
	{
		fprintf(stderr, "sliSoundPause(): sound index %d is out of range [0, %d)\n", sound, MAX_SOURCES - 1);
		exit(1);
	}
}

void sliSoundStop(int sound)
{
	if(sliIsIndexValid(sound))
	{
		alSourceStop(sources[sound]);
	}
	else
	{
		fprintf(stderr, "sliSoundStop(): sound index %d is out of range [0, %d)\n", sound, MAX_SOURCES - 1);
		exit(1);
	}
}

void sliSoundPauseAll()
{
	int i;
	int state;

	// pause all sounds that are playing (this includes sounds that are looping)
	for(i = 0; i < MAX_SOURCES; i ++)
	{
		alGetSourcei(sources[i], AL_SOURCE_STATE, &state);
		if(state == AL_PLAYING)
		{
			alSourcePause(sources[i]);
		}
	}
}

void sliSoundStopAll()
{
	int i;
	int state;

	// stop all sounds that are playing (this includes sounds that are looping or paused)
	for(i = 0; i < MAX_SOURCES; i ++)
	{
		alGetSourcei(sources[i], AL_SOURCE_STATE, &state);
		if(state == AL_PLAYING || state == AL_PAUSED)
		{
			alSourceStop(sources[i]);
		}
	}
}

void sliSoundResumeAll()
{
	int i;
	int state;

	// resumes all sounds that are paused
	for(i = 0; i < MAX_SOURCES; i ++)
	{
		alGetSourcei(sources[i], AL_SOURCE_STATE, &state);
		if(state == AL_PAUSED)
		{
			alSourcePlay(sources[i]);
		}
	}
}

int sliSoundPlaying(int sound)
{
	int state;
	int result = 0;

	if(sliIsIndexValid(sound))
	{
		alGetSourcei(sources[sound], AL_SOURCE_STATE, &state);
		result = state == AL_PLAYING;
	}
	else
	{
		fprintf(stderr, "sliSoundIsPlaying(): sound index %d is out of range [0, %d)\n", sound, MAX_SOURCES - 1);
		exit(1);
	}

	return result;
}

int sliSoundLooping(int sound)
{
	int state;
	ALint looping;
	int result = 0;

	if(sliIsIndexValid(sound))
	{
		alGetSourcei(sources[sound], AL_SOURCE_STATE, &state);
		alGetSourcei(sources[sound], AL_LOOPING, &looping);
		result = state == AL_PLAYING && looping == AL_TRUE;
	}
	else
	{
		fprintf(stderr, "sliSoundIsLooping(): sound index %d is out of range [0, %d)\n", sound, MAX_SOURCES - 1);
		exit(1);
	}

	return result;
}

// special thanks to http://www.dunsanyinteractive.com/blogs/oliver/?p=72 for this
void sliLoadWaveFormat(const char *filename, ALuint buffer)
{
	FILE *soundFile = NULL;
	struct WAVE_Format wave_format;
	struct RIFF_Header riff_header;
	struct WAVE_Data wave_data;
	uint8_t *data;

	ALsizei size;
	ALsizei frequency;
	ALenum format = 0;
	ALenum error;

	// attempt to open the file to see if it exists and is readable
	soundFile = fopen(filename, "rb");
	if (!soundFile)
	{
		fprintf(stderr, "sliLoadWaveFormat() could not load %s---does the file exist?\n", filename);
		exit(1);
	}

	// read in the first chunk into the struct
	fread(&riff_header, sizeof(struct RIFF_Header), 1, soundFile);

	// check for RIFF and WAVE tag in memeory
	if ((riff_header.chunkID[0] != 'R' || riff_header.chunkID[1] != 'I' || riff_header.chunkID[2] != 'F' || riff_header.chunkID[3] != 'F') ||
	    (riff_header.format[0] !=  'W' || riff_header.format[1] !=  'A' || riff_header.format[2] !=  'V' || riff_header.format[3] !=  'E'))
	{
		fprintf(stderr, "sliLoadWaveFormat() did not detect the RIFF/WAVE header combination in %s\n", filename);
		exit(1);
	}

	// read in the 2nd chunk for the wave info
	fread(&wave_format, sizeof(struct WAVE_Format), 1, soundFile);

	// make sure the wave format is something we can work with
	if (wave_format.subChunkID[0] != 'f' || wave_format.subChunkID[1] != 'm' || wave_format.subChunkID[2] != 't' || wave_format.subChunkID[3] != ' ')
	{
		fprintf(stderr, "sliLoadWaveFormat() detected invalid wave format in %s\n", filename);
		exit(1);
	}

	// check for extra parameters
	if (wave_format.subChunkSize > 16)
	{
		fseek(soundFile, sizeof(int16_t), SEEK_CUR);
	}

	// read in the the last byte of data before the sound file
	fread(&wave_data, sizeof(struct WAVE_Data), 1, soundFile);

	// check for data tag in memory
	if (wave_data.subChunkID[0] != 'd' || wave_data.subChunkID[1] != 'a' || wave_data.subChunkID[2] != 't' || wave_data.subChunkID[3] != 'a')
	{
		fprintf(stderr, "sliLoadWaveFormat() detected invalid data header in %s\n", filename);
		exit(1);
	}

	// allocate memory for wave data
	data = (uint8_t*)malloc(wave_data.subChunk2Size);

	// read in the sound data into the soundData variable
	if (!fread(data, wave_data.subChunk2Size, 1, soundFile))
	{
		fprintf(stderr, "sliLoadWaveFormat() could not load wave data in %s into struct\n", filename);
		exit(1);
	}

	// now we set the variables that we passed in with the data from the structs
	size = wave_data.subChunk2Size;
	frequency = wave_format.sampleRate;

	// the format is worked out by looking at the number of channels and the bits per sample.
	if (wave_format.numChannels == 1)
	{
		if (wave_format.bitsPerSample == 8 )
			format = AL_FORMAT_MONO8;
		else if (wave_format.bitsPerSample == 16)
			format = AL_FORMAT_MONO16;
	}
	else if (wave_format.numChannels == 2)
	{
		if (wave_format.bitsPerSample == 8 )
			format = AL_FORMAT_STEREO8;
		else if (wave_format.bitsPerSample == 16)
			format = AL_FORMAT_STEREO16;
	}

	// now we put our data into the OpenAL buffer and check for an error
	alBufferData(buffer, format, (void*)data, size, frequency);
	error = alGetError();
	if(error != AL_NO_ERROR)
	{
		fprintf(stderr, "sliLoadWaveFormat() encountered AL error %d when buffering %s\n", (int)error, filename);
		exit(1);
	}

	// clean up
	fclose(soundFile);
}

int sliGetFreeSource()
{
	int i = sliNextFreeSource;
	int state;
	int result = -1;

	// start at an index that is an educated guess for a sound that might be free
	do
	{
		// get the state of the next source that might be available
		alGetSourcei(sources[i], AL_SOURCE_STATE, &state);
		if(state == AL_STOPPED || state == AL_INITIAL)
		{
			result = i;
			sliNextFreeSource = (i + 1) % MAX_SOURCES;
		}
		else
		{
			i = (i + 1) % MAX_SOURCES;
		}
	}
	while(result == -1 && i != sliNextFreeSource);

	// return either -1 or the index of a source that can be used
	return result;
}

int sliIsIndexValid(int sourceIndex)
{
	return sourceIndex >= 0 && sourceIndex < MAX_SOURCES;
}
