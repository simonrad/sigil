#include "sl.h"

int main(int args, char *argv[])
{
	slWindow(500, 500, "SL Window");
	slSetBackColor(0.0, 0.0, 0.0);

	while(!slGetKey(SL_KEY_ESCAPE))
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
*/
			slSetForeColor(1.0, 1.0, 1.0, 1.0);
			slPoint(15, 200);

			slSetForeColor(1.0, 0.0, 0.0, 1.0);
			slLine(201, 202, 303, 202);

			slSetForeColor(0.0, 1.0, 0.0, 1.0);
			slLine(100, 100, 200, 200);

			slSetForeColor(0.0, 0.0, 1.0, 1.0);
			slLine(400, 400, 450, 400);

			slSetForeColor(0.0, 1.0, 1.0, 0.5);
			slSprite("png/planet-icon.png", 200, 200, 100, 100);

			slSetForeColor(0.5, 0.5, 0.5, 0.5);
			slLine(200, 0, 200, 300);

			slSetForeColor(1.0, 1.0, 0.0, 1.0);
			slSetTextAlign(SL_ALIGN_LEFT);
			slSetFontSize(12);
			slText(200, 100, "hello world!");

			slSetForeColor(0.0, 1.0, 1.0, 1.0);
			slSetTextAlign(SL_ALIGN_CENTER);
			slText(200, 150, "hello world!");

			slSetForeColor(1.0, 0.0, 1.0, 1.0);
			slSetTextAlign(SL_ALIGN_RIGHT);
			slText(200, 200, "hello world!");

			slSetFontSize(40);
			slSetTextAlign(SL_ALIGN_LEFT);
			slText(20, 20, "test font size");

			int x, y;
			if(slGetMouseButton(SL_MOUSE_BUTTON_LEFT))
			{
				slGetMousePos(&x, &y);
				slSetForeColor(1.0, 1.0, 0.0, 0.2);
				slCircleFill(x, y, 15.0, 15);
				slSetForeColor(1.0, 1.0, 0.0, 1.0);
				slCircleOutline(x, y, 15.0, 15);
			}

		//slPop();

		slRender();
	}

	slClose();

	return 0;
}
