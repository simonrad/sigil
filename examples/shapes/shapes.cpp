#include "sl.h"

// shapes
// demonstrates how to draw outlined and filled geometric shapes

int main(int args, char *argv[])
{
	// the width and height of our desired window in pixels; rendered objects use the same coordinate space,
	// with the bottom left-hand corner having the world-space coordinate of (0, 0)
	const int WINDOW_WIDTH = 500;
	const int WINDOW_HEIGHT = 500;

	// this initializes the sigil library and creates a window of the desired size
	slWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "SIGIL Shapes Example");

	// the colour of the background is specified using RGB values in the range [0.0, 1.0]
	slSetBackColor(0.0, 0.0, 0.0);		// black

	// the window will remain open (and the program will remain in this loop) until the user presses the 'X' to close
	while(!slShouldClose() && !slGetKey(SL_KEY_ESCAPE))
	{
		// draw the inner fill for a triangle
		slSetForeColor(0.5, 0.0, 0.0, 0.5);
		slTriangleFill(100.0, 450.0, 50.0, 50.0);

		// draw the outline for a triangle
		slSetForeColor(0.8, 0.0, 0.0, 0.8);
		slTriangleOutline(100.0, 450.0, 50.0, 50.0);

		// draw the inner fill for a rectangle
		slSetForeColor(0.5, 0.5, 0.0, 0.5);
		slRectangleFill(200.0, 450.0, 50.0, 50.0);

		// draw the outline for a rectangle
		slSetForeColor(0.8, 0.8, 0.0, 0.8);
		slRectangleOutline(200.0, 450.0, 50.0, 50.0);

		// draw the inner fill for a hexagon
		slSetForeColor(0.0, 0.5, 0.0, 0.5);
		slCircleFill(300.0, 450.0, 25.0, 6);

		// draw the outline for a hexagon
		slSetForeColor(0.0, 0.8, 0.0, 0.8);
		slCircleOutline(300.0, 450.0, 25.0, 6);

		// draw the inner fill for a circle
		slSetForeColor(0.0, 0.0, 0.5, 0.5);
		slCircleFill(400.0, 450.0, 25.0, 16);

		// draw the outline for a circle
		slSetForeColor(0.0, 0.0, 0.8, 0.8);
		slCircleOutline(400.0, 450.0, 25.0, 16);

		// draw some lines that move downwards and change colour
		for(int i = 0; i < 20; i ++)
		{
			slSetForeColor((float)i / 20.0, (float)i / 40.0, 0.0, 0.9);
			slLine(100.0, 400.0 - i * 10.0, 400.0, 400.0 - i * 10.0);
		}

		// now draw some pretty points whose (x, y) position determines their colour
		for(int x = 100; x <= 400; x += 3)
		{
			for(int y = 25; y <= 200; y += 3)
			{
				// shifting colours as we move across the array of points
				slSetForeColor((float)(x - 100) / 300.0,		// red contribution is determined by x position
							   (float)(y - 100) / 175.0,		// green contribution is determined by y position
							   0.0,								// blue contribution is always zero
							   1.0);							// alpha is full

				// draw our point at the specified coordinate
				slPoint(x, y);
			}
		}

		// display anything that SIGIL needs to as a result of drawing commands
		slRender();
	}

	// shut down our sigil window
	slClose();

	// program ends
	return 0;
}
