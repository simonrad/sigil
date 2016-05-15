#pragma once

#include "glm/glm.hpp"

class Object
{
private:
	glm::vec2 size;
	float radius;
	int tex;

	glm::vec2 oldPos;
	glm::vec2 pos;
	float angle;
	glm::vec2 dir;

protected:
	void setSize(glm::vec2 size);
	void setTex(int tex);

public:
	Object();
	virtual ~Object() = 0;

	virtual void update(float dt) = 0;
	virtual void render();

	void setOldPos(glm::vec2 oldPos);
	glm::vec2 getOldPos();

	void setPos(glm::vec2 pos);
	glm::vec2 getPos();

	void setAngle(float angle);
	float getAngle();

	glm::vec2 getDir();

	float getRadius();
};
