#include "ship.h"
#include "guns.h"

Ship::Ship()
{
	guns = new Guns();
}

Ship::~Ship()
{
	delete guns;
}

void Ship::addGun(Gun *gun)
{
	guns -> addGun(gun);
}

void Ship::controlGuns(bool trigger, float dt)
{
	guns -> controlFiring(getPos(), getAngle(), trigger, dt);
}
