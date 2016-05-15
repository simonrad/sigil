#pragma once

#include "object.h"

class Entity : public Object
{
private:
	float maxHealth;
	float health;

public:
	Entity();
	virtual ~Entity() = 0;

	float getHealthFactor();
	void dealDamage(float damage);
};
