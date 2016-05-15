#include "guns.h"
#include "gun.h"

#include "glm/glm.hpp"
using namespace glm;

#include <vector>
using namespace std;

Guns::Guns() { }
Guns::~Guns() { }

void Guns::addGun(Gun *gun)
{
	guns.push_back(gun);
}

void Guns::controlFiring(vec2 center, float angle, bool trigger, float dt)
{
	vector<Gun*>::iterator i;

	for(i = guns.begin(); i != guns.end(); i ++)
	{
		(*i) -> controlFiring(center, angle, trigger, dt);
	}
}
