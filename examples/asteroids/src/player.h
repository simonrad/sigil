#pragma once

#include "ship.h"

class Player : public Ship
{
private:
	int laserTex;

	float speed;

	void controlMotion(float dt);

public:
	Player();
	~Player();

	void update(float dt);
};
