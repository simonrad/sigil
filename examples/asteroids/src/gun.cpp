#include "gun.h"
#include "laser.h"
#include "world.h"

#include "sl/sl.h"

#include "glm/glm.hpp"
#include "glm/gtx/rotate_vector.hpp"
using namespace glm;

Gun::Gun(vec2 offset, float firingInterval, float laserSpeed, float laserDamage, int laserTex)
{
	this -> offset = offset;
	this -> firingInterval = firingInterval;

	this -> laserSpeed = laserSpeed;
	this -> laserDamage = laserDamage;
	this -> laserTex = laserTex;

	firingTimer = 0.0;

	sound = slLoadWAV("../wav/cannonfire-light-9.wav");
}

void Gun::controlFiring(vec2 center, float angle, bool trigger, float dt)
{
    if(trigger)
    {
		firingTimer -= dt;
		if(firingTimer <= 0.0)
		{
			firingTimer = firingInterval;
			fire(center, angle);
		}
    }
    else
    {
		firingTimer = 0.0;
    }
}

void Gun::fire(vec2 center, float angle)
{
	vec2 relativeOffset = rotate(offset, radians(angle));
	vec2 absolutePos = center + relativeOffset;

	World::getInstance() -> addLaser(absolutePos, angle, laserSpeed, laserDamage, laserTex);

	slSoundPlay(sound);
}
