#include "player.h"
#include "gun.h"
#include "world.h"

#include "sl/sl.h"

#include "glm/glm.hpp"
using namespace glm;

#include <iostream>
using namespace std;

#include <stdio.h>

Player::Player()
{
	laserTex = slLoadTexture("../png/laser-green.png");

	setPos(vec2(225, 40));
	setAngle(0.0);

	setSize(vec2(50.0, 50.0));
	setTex(slLoadTexture("../png/player-ship.png"));

	addGun(new Gun(vec2(7.0, 7.0), 0.18, 600.0, 1.0, laserTex));
	addGun(new Gun(vec2(-7.0, 7.0), 0.18, 600.0, 1.0, laserTex));
}

Player::~Player()
{

}

void Player::update(float dt)
{
	controlMotion(dt);
	controlGuns(slGetKey(' '), dt);
}

void Player::controlMotion(float dt)
{
	const float ACCEL = 400.0;
	const float MAX_SPEED = 300.0;

	bool keyPressed = false;
	vec2 pos = getPos();
	vec2 dir = vec2(0.0);

	if(slGetKey(SL_KEY_UP))
	{
		speed += ACCEL * dt;
		dir += vec2(0.0, 1.0);
		keyPressed = true;
	}

	if(slGetKey(SL_KEY_DOWN))
	{
		speed += ACCEL * dt;
		dir += vec2(0.0, -1.0);
		keyPressed = true;
	}

	if(slGetKey(SL_KEY_LEFT))
	{
		speed += ACCEL * dt;
		dir += vec2(-1.0, 0.0);
		keyPressed = true;
	}

	if(slGetKey(SL_KEY_RIGHT))
	{
		speed += ACCEL * dt;
		dir += vec2(1.0, 0.0);
		keyPressed = true;
	}

	if(!keyPressed)
	{
		speed -= ACCEL * dt;
		if(speed < 0.0) speed = 0.0;
	}

	if(speed > MAX_SPEED)
	{
		speed = MAX_SPEED;
	}

	pos += dir * speed * dt;
	setPos(pos);

}
