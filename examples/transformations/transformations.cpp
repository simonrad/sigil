#include "sl.h"

// transformations
// demonstrates how to use the SIGIL transformation matrix stack

// draw a line with one point at the origin
void drawLimb(float limbLength);

// draw a circle around the origin
void drawJoint(float length);

// main program
int main(int args, char *argv[])
{
	// the width and height of our desired window in pixels; rendered objects use the same coordinate space,
	// with the bottom left-hand corner having the world-space coordinate of (0, 0)
	const int WINDOW_WIDTH = 500;
	const int WINDOW_HEIGHT = 500;

	// indicates how long the previous frame took to render, in seconds
	float dt;

	// angle of parent limb that rotates around the center of the screen
	float angle = 0.0;

	// this initializes the sigil library and creates a window of the desired size
	slWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "SIGIL Transformations Example");

	// the colour of the background is specified using RGB values in the range [0.0, 1.0]
	slSetBackColor(0.0, 0.0, 0.0);		// black

	// the window will remain open (and the program will remain in this loop) until the user presses the 'X' to close
	while(!slShouldClose() && !slGetKey(SL_KEY_ESCAPE))
	{
		// grab our time multiplier
		dt = slGetDeltaTime();

		// increment the angle of the main limb
		angle += 20 * dt;

		// rotate the entire limb system around the center of the screen
		slPush();

			slTranslate(250, 250);					// move to the center of the screen
			slRotate(angle);						// entire system rotates around the center of the screen
			drawLimb(120);							// the length of the parent limb is 120
			drawJoint(120);

			// a child limb rotates around the end of the parent limb
			slPush();
				slTranslate(0, 120);				// translate to the end of the parent limb above (which has length 120)
				slRotate(angle * 2.0);				// rotate a little faster than the parent
				drawLimb(75);						// the length of this child limb is 75
				drawJoint(75);

				// another child limb below that, rotating around its parent
				slPush();
					slTranslate(0, 75);				// translate to the end of this limb's parent (which has length 75)
					slRotate(angle * 3.0);			// rotate even faster than our parent
					drawLimb(35);					// this limb has a length of 35
					drawJoint(35);
				slPop();

				// this is a sibling limb that rotates around the same joint as the limb we just rendered above
				slPush();
					slTranslate(0, 75);				// translate to the end of this limb's parent (which has length 75)
					slRotate(angle * -2.5);			// rotate a little slower than our sibling limb, and in reverse
					drawLimb(30);					// no need to draw a joint, since our sibling did that already

					// a final child limb attached to the sibling limb above
					slPush();
						slTranslate(0, 30);			// translate to the end of the latter sibling's limb
						slRotate(angle * 2.2);		// rotate around the joint, as before
						drawLimb(25);
						drawJoint(25);
					slPop();

				slPop();

			slPop();

		slPop();

		// display anything that SIGIL needs to as a result of drawing commands
		slRender();
	}

	// shut down our sigil window
	slClose();

	// program ends
	return 0;
}

// draw a line with one point at the origin
void drawLimb(float limbLength)
{
	slSetForeColor(1.0, 1.0, 1.0, 1.0);
	slLine(0, 0, 0, limbLength);
}

// draw a circle around the origin
void drawJoint(float limbLength)
{
	slSetForeColor(0.0, 0.5, 0.0, 0.5);
	slCircleFill(0, 0, limbLength * 0.2, 16);

	slSetForeColor(0.0, 0.9, 0.0, 0.9);
	slCircleOutline(0, 0, limbLength * 0.2, 16);
}
