#include "laser.h"

#include "sl/sl.h"

#include "glm/glm.hpp"
using namespace glm;

#include <iostream>
using namespace std;

Laser::Laser()
{
	life = 0.0;
}

Laser::~Laser() { }

void Laser::configure(vec2 pos, float angle, float speed, float damage, int tex)
{
	setPos(pos);
	setAngle(angle);
	setOldPos(pos - getDir());

	this -> speed = speed;
	this -> damage = damage;
	this -> tex = tex;

	life = 1.0;
}

bool Laser::isDead()
{
	return life <= 0.0;
}

void Laser::update(float dt)
{
	vec2 pos;

	pos = getPos();
	setOldPos(pos);

	pos = pos + getDir() * speed * dt;
	setPos(pos);

	life -= dt;
}

void Laser::render()
{
	const float SIZE = 55.0;

	vec2 pos = getPos();
	float angle = getAngle();

	slPush();
		slTranslate(pos.x, pos.y);
		slRotate(angle);
		slSprite(tex, 0.0, 0.0, SIZE, SIZE);
	slPop();
}

float Laser::getDamage()
{
	return damage;
}

void Laser::kill()
{
	life = 0.0;
}
