#pragma once

#include "entity.h"

#include "glm/glm.hpp"

class Prop : public Entity
{
private:
	float spin;

public:
	Prop(int tex, glm::vec2 size, float spin);
	virtual ~Prop();

	void update(float dt);
};
