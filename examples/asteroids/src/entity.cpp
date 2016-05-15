#include "entity.h"

Entity::Entity()
{
	maxHealth = 1.0;
	health = 1.0;
}

Entity::~Entity() { }

float Entity::getHealthFactor()
{
	return health / maxHealth;
}

void Entity::dealDamage(float damage)
{
	if(damage > 0)
	{
		health -= damage;
	}
}
