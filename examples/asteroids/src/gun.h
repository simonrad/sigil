#pragma once

#include "glm/glm.hpp"

class Gun
{
private:
	// properties related to the gun
	glm::vec2 offset;
	float firingInterval;

	// properties related to the laser the gun fires
	float laserSpeed;
	float laserDamage;
	int laserTex;

	int sound;

	// in-game changing properties
	float firingTimer;

	void fire(glm::vec2 center, float angle);

public:
	Gun(glm::vec2 offset, float firingInterval, float laserSpeed, float laserDamage, int laserTex);
	~Gun();

	void controlFiring(glm::vec2 center, float angle, bool trigger, float dt);
};
