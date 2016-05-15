#include "world.h"

int main(int args, char *argv[])
{
	World *world = World::getInstance();
	world -> run();

	delete world;

	return 0;
}
