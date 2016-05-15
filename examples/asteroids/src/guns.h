#pragma once

#include "glm/glm.hpp"

#include <vector>

class Gun;

class Guns
{
private:
    std::vector<Gun*> guns;

public:
	Guns();
	~Guns();

	void addGun(Gun *gun);

	void controlFiring(glm::vec2 center, float angle, bool trigger, float dt);
};
