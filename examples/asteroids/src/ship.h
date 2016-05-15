#pragma once

#include "entity.h"

class Guns;
class Gun;

class Ship : public Entity
{
private:
	Guns *guns;

protected:
	void addGun(Gun *gun);
	void controlGuns(bool trigger, float dt);

public:
	Ship();
	virtual ~Ship() = 0;
};
