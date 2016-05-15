#pragma once

#include "glm/glm.hpp"

class Cursor
{
public:
	// cursor can be in one of three mutually-exclusive states
	typedef enum
	{
		STATE_NORMAL,				// not hovering over any entities
		STATE_HOVERING,				// hovering over an entity
		STATE_LOCKED				// locked onto an enemy we're hovering over
	} CursorState;

	Cursor();
	~Cursor();

	void update(float dt);
	void render();

	void setState(CursorState state, float targetRadius);

	glm::vec2 getPos();
	glm::vec2 getMotion();

private:
	glm::vec2 oldPos;
	glm::vec2 pos;
	glm::vec2 motion;
	float angle;

	int tex;

	CursorState state;
	float targetRadius;
};
