#include "sl.h"

#include <cstdio>

// sound
// demonstrates how to play, loop, pause, and stop WAVE sounds

int main(int args, char *argv[])
{
	// the width and height of our desired window in pixels; rendered objects use the same coordinate space,
	// with the bottom left-hand corner having the world-space coordinate of (0, 0)
	const int WINDOW_WIDTH = 500;
	const int WINDOW_HEIGHT = 500;

	// this initializes the sigil library and creates a window of the desired size
	slWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "SIGIL Sound Example");

	// the colour of the background is specified using RGB values in the range [0.0, 1.0]
	slSetBackColor(0.0, 0.0, 0.0);		// black

	slSetFont("../ttf/white_rabbit.ttf", 12);

	int sounds[12];
	char soundName[16];
	int i;

	for(i = 0; i <= 9; i ++)
	{
		sprintf(soundName, "../wav/%d.wav", i);
		sounds[i] = slLoadWAV(soundName);
	}
	sounds[10] = slLoadWAV("../wav/star.wav");
	sounds[11] = slLoadWAV("../wav/pound.wav");

	// the window will remain open (and the program will remain in this loop) until the user presses the 'X' to close
	while(!slShouldClose() && !slGetKey(SL_KEY_ESCAPE))
	{
		slText(250, 250, "hello world!");

		if(slGetKey('A'))
		{
			printf("sound %d\n", slSoundPlay(sounds[0]));
		}

		// display anything that SIGIL needs to as a result of drawing commands
		slRender();
	}

	// shut down our sigil window
	slClose();

	// program ends
	return 0;
}
