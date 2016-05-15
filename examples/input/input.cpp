#include "sl.h"

#include <cstdio>

// input
// demonstrates some simple text, mouse/keyboard input, and delta motion timing

// minimum and maximum allowed number of bubbles in the mouse-following chain
#define MIN_CIRCLES 5
#define MAX_CIRCLES 50

// linear interpolation between two values, with clamping to prevent overshoot
float moveTowards(float current, float target, float step);

// main program
int main(int args, char *argv[])
{
	// the width and height of our desired window in pixels; rendered objects use the same coordinate space,
	// with the bottom left-hand corner having the world-space coordinate of (0, 0)
	const int WINDOW_WIDTH = 700;
	const int WINDOW_HEIGHT = 600;

	// circle properties
	const float CIRCLE_SPEED = 20.0;		// how fast the circles move towards their target, in units per second
	const float CIRCLE_RADIUS = 40.0;		// radius of largest circle

	// the number of actual bubbles we're using
	int numCircles = MAX_CIRCLES / 2;

	// current mouse position
	int mouseX;
	int mouseY;

	// define the properties for each bubble in the chain of bubbles following the mouse
	float circleXCoords[MAX_CIRCLES];	// x position
	float circleYCoords[MAX_CIRCLES];	// y position
	float tailFactor;					// factor from 1.0 to 0.0 as we iterate through the bubble chain, for computing values below
	float speed;						// speed of current bubble when catching up to the next one
	float radius;						// bubbles scale down in size as we go further down the bubble chain
	int vertices;						// bubble vertex count scales down as we go further down the bubble chain
	int i;

	// for explanatory text to user
	char str[128];

	// key reading
	bool keyW = false;
	bool keyS = false;

	// indicates how long the previous frame took to render, in seconds
	float dt;

	// initialize the position of our circles to the center of the world
	for(i = 0; i < MAX_CIRCLES; i ++)
	{
		circleXCoords[i] = 350.0;
		circleYCoords[i] = 300.0;
	}

	// this initializes the sigil library and creates a window of the desired size
	slWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "SIGIL Input Example");

	// the colour of the background is specified using RGB values in the range [0.0, 1.0]
	slSetBackColor(0.0, 0.0, 0.0);		// black

	// set the font we want to use
	slSetFont("../ttf/white_rabbit.ttf", 16);

	// the window will remain open (and the program will remain in this loop) until the user presses the 'X' to close
	while(!slShouldClose() && !slGetKey(SL_KEY_ESCAPE))
	{
		// grab our time increment
		dt = slGetDeltaTime();

		// read the 'W' key to increase the number of circles
		if(slGetKey('W'))
		{
			if(!keyW)
			{
				keyW = true;
				numCircles ++;
				if(numCircles > MAX_CIRCLES) numCircles = MAX_CIRCLES;
			}
		}
		else
		{
			keyW = false;
		}

		// read the 'S' key to decrease the number of circles
		if(slGetKey('S'))
		{
			if(!keyS)
			{
				keyS = true;
				numCircles --;
				if(numCircles < MIN_CIRCLES) numCircles = MIN_CIRCLES;
			}
		}
		else
		{
			keyS = false;
		}

		// make the first circle follow the mouse
		slGetMousePos(&mouseX, &mouseY);
		circleXCoords[0] = mouseX;
		circleYCoords[0] = mouseY;

		// the other circles follow the one ahead of it
		for(i = 1; i < MAX_CIRCLES; i ++)
		{
			speed = CIRCLE_SPEED * dt;
			circleXCoords[i] = moveTowards(circleXCoords[i], circleXCoords[i - 1], speed);
			circleYCoords[i] = moveTowards(circleYCoords[i], circleYCoords[i - 1], speed);
		}

		// now render our circles
		for(i = 0; i < numCircles; i ++)
		{
			// compute a colour and size factor for the bubbles
			tailFactor = (float)i / (float)MAX_CIRCLES;
			radius = CIRCLE_RADIUS * (1.0 - tailFactor);
			vertices = 15 - (10.0 * tailFactor);					// larger bubbles require more vertices

			// render the solid circle fill
			slSetForeColor(0.0, 1.0 - tailFactor, tailFactor, 0.3);
			slCircleFill(circleXCoords[i],							// x position
						 circleYCoords[i],							// y position
						 radius,									// bubbles start large and then get small
						 vertices);									// larger bubbles require more vertices

			// render the circle outline
			slSetForeColor(0.0, 1.0 - tailFactor, tailFactor, 0.5);
			slCircleOutline(circleXCoords[i],						// x position
							circleYCoords[i],						// y position
							radius,									// bubbles start large and then get small
							vertices);								// larger bubbles require more vertices
		}

		// show some explanatory text
		sprintf(str, "move the mouse around\n\n%d bubbles in chain; use W to increase or S to decrease", numCircles);
		slSetForeColor(1.0, 1.0, 1.0, 1.0);
		slText(10, WINDOW_HEIGHT - 20, str);

		// display anything that SIGIL needs to as a result of drawing commands
		slRender();
	}

	// shut down our sigil window
	slClose();

	// program ends
	return 0;
}

// linear interpolation between two values, with clamping to prevent overshoot
float moveTowards(float current, float target, float step)
{
	float result = current + (target - current) * step;
	if(current < target && result > target) result = target;
	else if(current > target && result < target) result = target;
	return result;
}
