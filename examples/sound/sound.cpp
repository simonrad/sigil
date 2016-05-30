#include "sl.h"

#include <cstdio>

// sound
// demonstrates how to play, loop, pause, and stop WAVE sounds

int main(int args, char *argv[])
{
	// the width and height of our desired window in pixels; rendered objects use the same coordinate space,
	// with the bottom left-hand corner having the world-space coordinate of (0, 0)
	const int WINDOW_WIDTH = 380;
	const int WINDOW_HEIGHT = 140;

	// track the keys that are currently down
	bool digitKeysDown[10];
	bool musicPlayKeyDown = false;
	bool musicLoopKeyDown = false;
	bool pauseKeyDown = false;
	bool resumeKeyDown = false;
	bool stopKeyDown = false;

	// SL sound loading
	char soundName[32];					// to build filenames of the phone beeps
	int digitSounds[10];				// for phone digit beeps we load
	int music;							// for the music loop we load
	int i;

	// this initializes the sigil library and creates a window of the desired size
	slWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "SIGIL Sound Example");

	// the colour of the background is specified using RGB values in the range [0.0, 1.0]
	slSetBackColor(0.0, 0.0, 0.0);		// black

	// set a pleasant-looking font
	slSetFont("../ttf/white_rabbit.ttf", 12);

	// load the sounds corresponding to phone number beeps
	for(i = 0; i <= 9; i ++)
	{
#ifdef _MSC_VER
		sprintf_s(soundName, "../wav/%d.wav", i);
#else
		sprintf(soundName, "../wav/%d.wav", i);
#endif
		digitSounds[i] = slLoadWAV(soundName);
	}

	// load the music; this is licensed under the CC0 1.0 License
	music = slLoadWAV("../wav/220427__massacresoundz__intro-loop-hs-24-02-2014.wav");

	// the window will remain open (and the program will remain in this loop) until the user presses the 'X' to close
	while(!slShouldClose() && !slGetKey(SL_KEY_ESCAPE))
	{
		// print some introductory text giving some options to the user
		slText(20, 120, "Press 0-9 for the corresponding phone beeps");
		slText(20, 100, "Press M to play music once");
		slText(20, 80, "Press L to loop music");
		slText(20, 60, "Press P to pause all sounds");
		slText(20, 40, "Press R to resume any paused sounds");
		slText(20, 20, "Press S to stop all sounds");

		// play phone digit beep when user presses corresponding key
		for(i = 0; i <= 9; i ++)
		{
			if(slGetKey('0' + i))
			{
				if(!digitKeysDown[i])
				{
					slSoundPlay(digitSounds[i]);
					digitKeysDown[i] = true;
				}
			}
			else
			{
				digitKeysDown[i] = false;
			}
		}

		// M key plays music once
		if(slGetKey('M'))
		{
			if(!musicPlayKeyDown)
			{
				slSoundPlay(music);
				musicPlayKeyDown = true;
			}
		}
		else
		{
			musicPlayKeyDown = false;
		}

		// L key loops music
		if(slGetKey('L'))
		{
			if(!musicLoopKeyDown)
			{
				slSoundLoop(music);
				musicLoopKeyDown = true;
			}
		}
		else
		{
			musicLoopKeyDown = false;
		}

		// P key pauses everything
		if(slGetKey('P'))
		{
			if(!pauseKeyDown)
			{
				slSoundPauseAll();
				pauseKeyDown = true;
			}
		}
		else
		{
			pauseKeyDown = false;
		}

		// R key resumes everything that was paused
		if(slGetKey('R'))
		{
			if(!resumeKeyDown)
			{
				slSoundResumeAll();
				resumeKeyDown = true;
			}
		}
		else
		{
			resumeKeyDown = false;
		}

		// S key stops all sounds (including paused ones)
		if(slGetKey('S'))
		{
			if(!stopKeyDown)
			{
				slSoundStopAll();
				stopKeyDown = true;
			}
		}
		else
		{
			stopKeyDown = false;
		}

		// display anything that SIGIL needs to as a result of drawing commands
		slRender();
	}

	// shut down our sigil window
	slClose();

	// program ends
	return 0;
}
