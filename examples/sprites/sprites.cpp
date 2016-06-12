#include "sl.h"

#include <cstdlib>

// sprites
// demonstrates how to display images as sprites

#define NUM_FLARES 30

int main(int args, char *argv[])
{
	// the width and height of our desired window in pixels; rendered objects use the same coordinate space,
	// with the bottom left-hand corner having the world-space coordinate of (0, 0)
	const int WINDOW_WIDTH = 500;
	const int WINDOW_HEIGHT = 500;

	// indicates how long the previous frame took to render, in seconds
	double dt;

	// scrolling values for our main checkered background
	double scrollX = 0.0;
	double scrollY = 0.0;

	// positions and sizes of sprites
	double flareX[NUM_FLARES];
	double flareY[NUM_FLARES];
	double flareSizes[NUM_FLARES];
	int i;

	// our texture identifiers
	int patternTex;
	int flareTex;

	// this initializes the sigil library and creates a window of the desired size
	slWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "SIGIL Sprites Example", false);

	// the colour of the background is specified using RGB values in the range [0.0, 1.0]
	slSetBackColor(0.0, 0.0, 0.0);		// black

	// load up some textures
	patternTex = slLoadTexture("../png/pattern.png");
	flareTex = slLoadTexture("../png/flare.png");

	// assign random flare positions and sizes
	for(i = 0; i < NUM_FLARES; i ++)
	{
		flareX[i] = rand() % 500;
		flareY[i] = rand() % 500;
		flareSizes[i] = 40 + rand() % 170;
	}

	// the window will remain open (and the program will remain in this loop) until the user presses the 'X' to close
	while(!slShouldClose() && !slGetKey(SL_KEY_ESCAPE))
	{
        // grab our time multiplier
		dt = slGetDeltaTime();

		// scroll our background a little
		scrollX += dt;
		scrollY += dt * 2.0;

		// draw the main checkered background by tiling a simple 2x2 pattern
		slSetAdditiveBlend(false);
		slSetForeColor(0.0, 0.4, 0.7, 0.6);
		slSetSpriteTiling(20.0, 20.0);					// tile the pattern
		slSetSpriteScroll(scrollX, scrollY);			// scroll the texture on the sprite
		slSprite(patternTex, 250, 250, 500, 500);

		// prepare our sprite settings for the flare bubbles we will render
		slSetAdditiveBlend(true);
		slSetForeColor(0.5, 0.5, 0.1, 0.6);
		slSetSpriteTiling(1.0, 1.0);					// reset the tiling to default
		slSetSpriteScroll(0.0, 0.0);					// ...and don't scroll, either

		// render the glowing flares
		for(i = 0; i < NUM_FLARES; i ++)
		{
			// flares move all the way up and then reset at the bottom of the world
			flareY[i] += 20.0 * dt;
			if(flareY[i] > 500.0 + flareSizes[i] / 2.0)
			{
				flareY[i] = -flareSizes[i] / 2.0;
			}

			// render the flare at its appropriate position
			slSprite(flareTex, flareX[i], flareY[i], flareSizes[i], flareSizes[i]);
		}

		// display anything that SIGIL needs to as a result of drawing commands
		slRender();
	}

	// shut down our sigil window
	slClose();

	// program ends
	return 0;
}
