#pragma once

#include "laser.h"

#include "glm/glm.hpp"

#include <vector>

#define MAX_LASERS 50

class Object;
class Entity;
class Player;
class Cursor;

class World
{
private:
	static World *instance;

	// actual list of all objects in game
	std::vector<Object*> objects;

	// used for collision
	std::vector<Entity*> entities;

	// our backdrop texture
	int starsTex;

	// pool of lasers to use when needed, to avoid run-time allocation
	Laser lasers[MAX_LASERS];
	int numActiveLasers;			// how many lasers we're actually using
	int nextFreeLaserIndex;			// a decent guess as to the index of an unused laser object

	// pool of asteroids to use when needed, to avoid run-time allocation


	Player *player;

	World();

	void update(float dt);
	void render();

	void updateObjects(float dt);

	void updateLasers(float dt);
	void renderLasers();

	void handleCollisions();

	Laser *getUnusedLaser();

public:
	static const int WORLD_SIZE_X;
	static const int WORLD_SIZE_Y;

	static World *getInstance();

	~World();

	void addEntity(Entity *Entity);
	void addLaser(glm::vec2 pos, float angle, float speed, float damage, int tex);

	glm::vec2 getCursorWorldPos();
	glm::vec2 getCursorMotion();

	void run();
};
