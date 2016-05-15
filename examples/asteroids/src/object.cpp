#include "object.h"

#include "sl/sl.h"

#include "glm/glm.hpp"
using namespace glm;

Object::Object() { }
Object::~Object() { }

void Object::update(float dt) { }

void Object::render()
{
	vec2 pos = getPos();
	float angle = getAngle();

	slPush();
		slTranslate(pos.x, pos.y);
		slRotate(angle);
		slSetForeColor(1.0, 1.0, 1.0, 1.0);
		slSprite(tex, 0.0, 0.0, size.x, size.y);
	slPop();
}

void Object::setTex(int tex)
{
	this -> tex = tex;
}

void Object::setSize(vec2 size)
{
	this -> size = size;
	radius = max(size.x, size.y) / 2.0;
}

void Object::setOldPos(vec2 oldPos)
{
	this -> oldPos = oldPos;
}

vec2 Object::getOldPos()
{
	return oldPos;
}

void Object::setPos(vec2 pos)
{
	this -> pos = pos;
}

vec2 Object::getPos()
{
	return pos;
}

void Object::setAngle(float angle)
{
	float rads = radians(360.0 - angle);

	this -> angle = angle;
	dir = vec2(sinf(rads), cosf(rads));
}

float Object::getAngle()
{
	return angle;
}

vec2 Object::getDir()
{
	return dir;
}

float Object::getRadius()
{
	return radius;
}
