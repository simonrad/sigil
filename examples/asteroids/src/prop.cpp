#include "prop.h"

#include "sl/sl.h"

#include "glm/glm.hpp"
using namespace glm;

Prop::Prop(int tex, vec2 size, float spin)
{
	setTex(tex);
	setSize(size);

	this -> spin = spin;
}

Prop::~Prop() { }

void Prop::update(float dt)
{
	setAngle(getAngle() + spin * dt);
}
