#include "cursor.h"

#include "sl/sl.h"

#include "glm/glm.hpp"
using namespace glm;

Cursor::Cursor()
{
	tex = slLoadTexture("../png/reticle-small.png");
}

Cursor::~Cursor() { }

void Cursor::update(float dt)
{
	const float ROTATE_RATE = -150.0 * dt;

	int mouseX, mouseY;

	// compute the mouse screen position and how much it has moved
	slGetMousePos(&mouseX, &mouseY);
	oldPos = pos;
	pos = vec2(mouseX, mouseY);
	motion = pos - oldPos;

	if(state != STATE_NORMAL)
	{
		angle += ROTATE_RATE;
	}
}

void Cursor::render()
{
	const float SIZE = 16;
	const float PIECE_OFFSET = 12.0 + targetRadius;
	const float LOCK_OFFSET = 15 + targetRadius;

	int i;

	for(i = 0; i < 4; i ++)
	{
		slPush();
			slTranslate(pos.x, pos.y);
			slRotate(angle += 90.0);
			slTranslate(PIECE_OFFSET, 0.0);
			slRotate(45.0);
			slSprite(tex, 0.0, 0.0, SIZE, SIZE);
		slPop();
	}

	if(state == STATE_LOCKED)
	{
		for(i = 0; i < 4; i ++)
		{
			slPush();
				slTranslate(pos.x, pos.y);
				slRotate(angle += 90.0);
				slTranslate(LOCK_OFFSET, 0.0);
				slRotate(45.0);
				slSprite(tex, 0.0, 0.0, SIZE - 6, SIZE - 6);
			slPop();
		}
	}
}

void Cursor::setState(CursorState state, float targetRadius)
{
	this -> state = state;
	this -> targetRadius = targetRadius;

	if(state == STATE_NORMAL)
	{
		angle = 0.0;
	}
}

vec2 Cursor::getPos()
{
	return pos;
}

vec2 Cursor::getMotion()
{
	return motion;
}
