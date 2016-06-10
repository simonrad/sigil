#include "sl.h"

#include <cstdlib>

// text
// demonstrates how to load and display text of varying point sizes

int main(int args, char *argv[])
{
	// the width and height of our desired window in pixels; rendered objects use the same coordinate space,
	// with the bottom left-hand corner having the world-space coordinate of (0, 0)
	const int WINDOW_WIDTH = 600;
	const int WINDOW_HEIGHT = 300;

	// this stores our font ID
	int font;

	// this initializes the sigil library and creates a window of the desired size
	slWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "SIGIL Text Example", false);

	// the colour of the background is specified using RGB values in the range [0.0, 1.0]
	slSetBackColor(0.0, 0.0, 0.0);		// black

	// set our active font
	font = slLoadFont("../ttf/euphorigenic.ttf");
	slSetFont(font, 28);

	// the window will remain open (and the program will remain in this loop) until the user presses the 'X' to close
	while(!slShouldClose() && !slGetKey(SL_KEY_ESCAPE))
	{
        // print the title out
        slSetTextAlign(SL_ALIGN_CENTER);
        slSetFontSize(28);
        slText(300, 250, "A Tale of Two Cities");

        // print the author
        slSetFontSize(20);
        slText(300, 200, "Charles Dickens");

        // print the first paragraph
        slSetTextAlign(SL_ALIGN_LEFT);
        slSetFontSize(14);
        slText(20, 150, "It was the best of times, it was the worst of times, it was the age of wisdom, it was the age\n" \
						"of foolishness, it was the epoch of belief, it was the epoch of incredulity, it was the season\n" \
						"of Light, it was the season of Darkness, it was the spring of hope, it was the winter of\n" \
						"despair, we had everything before us, we had nothing before us, we were all going direct to\n" \
						"Heaven, we were all going direct the other way - in short, the period was so far like the\n" \
						"present period, that some of its noisiest authorities insisted on its being received, for\n" \
						"good or for evil, in the superlative degree of comparison only.");

		// lastly, demonstrate right-aligned text
		slSetTextAlign(SL_ALIGN_RIGHT);
		slText(580, 20, "[continued in actual book...]");

		// display anything that SIGIL needs to as a result of drawing commands
		slRender();
	}

	// shut down our sigil window
	slClose();

	// program ends
	return 0;
}

