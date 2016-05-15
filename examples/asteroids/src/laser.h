#pragma once

#include "object.h"

#include "glm/glm.hpp"

class Laser : public Object
{
private:
	float speed;
	float damage;
	int tex;

	float life;

public:
	Laser();
	~Laser();

	void configure(glm::vec2 pos, float angle, float speed, float damage, int tex);
	bool isDead();

	void update(float dt);
	void render();

	float getDamage();

	void kill();
};
