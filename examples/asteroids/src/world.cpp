#include "world.h"
#include "entity.h"
#include "prop.h"
#include "laser.h"
#include "player.h"
#include "cursor.h"
#include "object.h"
#include "util.h"

#include "sl/sl.h"

#include "glm/glm.hpp"
using namespace glm;

#include <vector>
#include <iostream>
using namespace std;

const int World::WORLD_SIZE_X = 450;
const int World::WORLD_SIZE_Y = 650;

World *World::instance = NULL;

World::World()
{
	numActiveLasers = 0;
	nextFreeLaserIndex = 0;
}

World::~World()
{
	// delete game objects
}

World *World::getInstance()
{
	if(instance == NULL)
	{
		instance = new World();
	}

	return instance;
}

void World::addEntity(Entity *entity)
{
	objects.push_back(entity);
	entities.push_back(entity);
}

void World::addLaser(vec2 pos, float angle, float speed, float damage, int tex)
{
	Laser *laser = getUnusedLaser();
	if(laser)
	{
		laser -> configure(pos, angle, speed, damage, tex);
	}
}

void World::run()
{
	float dt;				// time delta tracking for smooth frame rate

	// open our game window
	slWindow(WORLD_SIZE_X, WORLD_SIZE_Y, "Asteroids");
	slSetBackColor(1.0, 1.0, 1.0);

	starsTex = slLoadTexture("../png/stars.png");

	// load up our game objects
	player = new Player();
	addEntity(player);

	// entity for testing
	Prop *asteroid = new Prop(slLoadTexture("../png/asteroid.png"), vec2(64.0, 64.0), 10.0);
	addEntity(asteroid);
	asteroid -> setPos(vec2(-200.0, 0.0));

	// main loop
	while(!slGetKey(SL_KEY_ESCAPE) && !slShouldClose())
	{
		// update the scene and render the objects in it
		dt = slGetDeltaTime();

		update(dt);
		render();

		// render anything leftover in our SL buffer
		slRender();
	}

	// shut down SIGIL and close the window
	slClose();
}

void World::update(float dt)
{
	updateObjects(dt);
	updateLasers(dt);
	handleCollisions();
}

void World::updateObjects(float dt)
{
	vector<Object*>::iterator i;

	// update all of our game objects
	for(i = objects.begin(); i != objects.end(); i ++)
	{
		(*i) -> update(dt);
	}
}

void World::render()
{
	const int CENTER_X = WORLD_SIZE_X / 2;
	const int CENTER_Y = WORLD_SIZE_Y / 2;

	vector<Object*>::iterator i;

	slSetForeColor(1.0, 1.0, 1.0, 1.0);
	slSetSpriteTiling(2.0, 3.0);
	slSprite(starsTex, CENTER_X, CENTER_Y, WORLD_SIZE_X, WORLD_SIZE_Y);
	slSetSpriteTiling(1.0, 1.0);

	for(i = objects.begin(); i != objects.end(); i ++)
	{
		(*i) -> render();
	}

	// render the game lasers
	renderLasers();
}

void World::updateLasers(float dt)
{
	int i;
	Laser *curr = &lasers[0];

	numActiveLasers = 0;

	for(i = 0; i < MAX_LASERS; i ++)
	{
		//cout << "numActiveLasers: " << numActiveLasers << " and i: " << i << endl;
		if(!curr -> isDead())
		{
			curr -> update(dt);
			if(!curr -> isDead())
			{
				numActiveLasers ++;
			}
		}

		// advance to the next laser object
		curr ++;
	}
}

void World::renderLasers()
{
	int i = 0;
	int numRendered = 0;
	Laser *curr = &lasers[i];

	while(numRendered < numActiveLasers && i < MAX_LASERS)
	{
		if(!curr -> isDead())
		{
			curr -> render();
			numRendered ++;
		}

		// advance to the next laser object
		i ++;
		curr ++;
	}
}

void World::handleCollisions()
{
	vector<Entity*>::iterator i;
	Entity *entity;

	int j;
	int numLasersChecked;
	Laser *laser;
	vec2 intersect;

	for(i = entities.begin(); i != entities.end(); i ++)
	{
		entity = *i;

		j = 0;
		numLasersChecked = 0;
		laser = &lasers[0];
		while(numLasersChecked < numActiveLasers && j < MAX_LASERS)
		{
			// laser must first be alive
			if(!laser -> isDead())
			{
				// we don't need to test objects we are moving away from (including the ship that fired the laser)
				if(dot(laser -> getDir(), (entity -> getPos() - laser -> getPos())) > 0.0)
				{
					// use a fast ray-sphere intersection test to see if the laser crossed over into the entity's radius
					if(rayIntersectsSphere(laser -> getOldPos(),
										   laser -> getPos(),
										   entity -> getPos(),
										   entity -> getRadius(),
										   intersect))
					{
						laser -> kill();
					}
				}

				numLasersChecked ++;
			}

			// advance to the next laser bolt
			j ++;
			laser++;
		}
	}
}

Laser *World::getUnusedLaser()
{
	int i = nextFreeLaserIndex;
	Laser *result = NULL;

	// start at an index that is an educated guess for a laser that might be free
	do
	{
		if(lasers[i].isDead())
		{
			// we found a free laser, so record it and start the next search at the spot after this in the pool
			result = &lasers[i];
			nextFreeLaserIndex = (i + 1) % MAX_LASERS;
		}
		else
		{
			// advance to the next laser
			i = (i + 1) % MAX_LASERS;
		}
	}
	while(!result && i != nextFreeLaserIndex);

	return result;
}
