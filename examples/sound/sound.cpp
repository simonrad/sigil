#include "sl.h"

#include <cstdio>

// sound
// demonstrates how to play, loop, pause, and stop WAVE sounds

int main(int args, char *argv[])
{
	// the width and height of our desired window in pixels; rendered objects use the same coordinate space,
	// with the bottom left-hand corner having the world-space coordinate of (0, 0)
	const int WINDOW_WIDTH = 300;
	const int WINDOW_HEIGHT = 200;

	// track the keys that are currently down
	bool digitKeysDown[10];
	bool musicKeyDown = false;
	bool pauseKeyDown = false;
	bool resumeKeyDown = false;
	bool stopKeyDown = false;

	// SL sound loading
	char soundName[32];
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
		sprintf(soundName, "../wav/%d.wav", i);
		digitSounds[i] = slLoadWAV(soundName);
	}

	// load the music; this is licensed under the CC0 1.0 License
	music = slLoadWAV("../wav/220427__massacresoundz__intro-loop-hs-24-02-2014.wav");

	// the window will remain open (and the program will remain in this loop) until the user presses the 'X' to close
	while(!slShouldClose() && !slGetKey(SL_KEY_ESCAPE))
	{
		// print some introductory text giving some options to the user
		slText(50, 450, "Press 0-9 for the corresponding phone beeps");
		slText(50, 430, "Press M to start playing music");
		slText(50, 410, "Press P to pause all sounds");
		slText(50, 390, "Press R to resume any paused sounds");
		slText(50, 390, "Press S to stop all sounds");

		// play phone digit beep when user presses corresponding key
		for(i = 0; i <= 9; i ++)
		{
			if(slGetKey('0' + i) && !digitKeysDown[i])
			{
				slSoundPlay(digitSounds[i]);
				digitKeysDown[i] = true;
			}
			else
			{
				digitKeysDown[i] = false;
			}
		}

		// M key starts music
		if(slGetKey('M') && !musicKeyDown)
		{
			slSoundPlay(music);
			musicKeyDown = true;
		}
		else
		{
			musicKeyDown = false;
		}

		// P key pauses everything
		if(slGetKey('P') && !pauseKeyDown)
		{
			slSoundPauseAll();
			pauseKeyDown = true;
		}
		else
		{
			pauseKeyDown = false;
		}

		// R key resumes everything that was paused
		if(slGetKey('R') && !resumeKeyDown)
		{
			slSoundResumeAll();
			resumeKeyDown = true;
		}
		else
		{
			resumeKeyDown = false;
		}

		// S key stops all sounds (including paused ones)
		if(slGetKey('S') && !stopKeyDown)
		{
			slSoundStopAll();
			stopKeyDown = true;
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
