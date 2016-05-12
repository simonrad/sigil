#include "sl.h"

int main(int args, char *argv[])
{
	// the width and height of our desired window in pixels; rendered objects use the same coordinate space
	const int WINDOW_WIDTH = 500;
	const int WINDOW_HEIGHT = 500;

	// this initializes the sigil library and creates a window of the desired size
	slWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "SIGIL Shapes Example");

	// the colour of the background is specified using RGB values in the range [0.0, 1.0]
	slSetBackColor(0.0, 0.0, 0.0);		// black

	// the window will remain open (and the program will remain in this loop) until the user presses the 'X' to close
	while(!slShouldClose())
	{
		// draw the inner fill for a triangle
		slSetForeColor(0.5, 0.5, 0.0, 0.5);
		slRectangleFill(100, 100, 50, 50);

		// draw the outline for a triangle
		slSetForeColor(0.8, 0.8, 0.0, 0.8);
		slRectangleOutline(100.0, 100.0, 50.0, 50.0);

		// display anything that SIGIL needs to as a result of drawing commands
		slRender();
	}

	// shut down our sigil window
	slClose();

	// program ends
	return 0;
}
