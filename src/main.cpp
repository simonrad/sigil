#include "sl.h"

int main(int args, char *argv[])
{
	slWindow(500, 500, "SL Window");
	slSetBackColor(0.0, 0.0, 0.0);

	while(true)
	{
		slSetForeColor(0.3, 0.0, 0.0, 1.0);

		//slPush();
			//slTranslate(250, 150);
/*
			slPush();
				slTranslate(0.0, 0.0);
				slRectangleFill(400.0, 400.0);
			slPop();

			slSetForeColor(1.0, 0.0, 0.0, 1.0);

			slPush();
				slRectangleOutline(1.0, 0.75);
			slPop();

			slSetForeColor(0.0, 0.3, 0.1, 0.2);

			slCircleFill(0.25, 15);
			slCircleFill(0.2, 5);

			slCircleOutline(0.4, 15);
			slCircleOutline(0.3, 7);
*/
			slSetForeColor(1.0, 1.0, 1.0, 1.0);
			slPush();
				slTranslate(100, 250);
				slPoint(0, 0);
			slPop();

			slSetForeColor(0.0, 1.0, 1.0, 1.0);
			slPush();
				slTranslate(200, 200);
				slPoint(0, 0);
			slPop();

			slPoint(205, 200);

			slSetForeColor(1.0, 0.0, 0.0, 1.0);
			slLine(201, 202, 303, 202);

			slSetForeColor(0.0, 1.0, 0.0, 1.0);
			slLine(100, 100, 200, 200);

			slSetForeColor(0.0, 0.0, 1.0, 1.0);
			slLine(400, 400, 450, 400);

<<<<<<< HEAD
			slSprite("png/planet-icon.png", 200, 200, 100, 100);

			slText(200, 400, "hello world!");


=======
>>>>>>> 4a968528c09f6a816265cfb5ba447ffd1ac8dc2a
		//slPop();

		slRender();
	}

	slClose();

	return 0;
}
