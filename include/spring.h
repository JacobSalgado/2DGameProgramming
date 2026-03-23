#ifndef __SPRING_H__
#define __SPRING_H__

#include "entity.h"

typedef struct
{
	float launch_velocity;
	Uint8 triggered;
}SpringData;

/**
* @brief the location to spawn the spring
* @param x the x-coordinate
* @param y the y-coordinate
*/
Entity* spring_new(float x, float y);

void spring_think(Entity* self);

void spring_update(Entity* self);

void spring_free(Entity* self);

/**
* @brief launches the entity upward after overlapping
* @param spring the spring to launch the player
* @param player the player being launched
* @return 1 if launching, 0 otherwise
*/
int spring_activate(Entity* spring, Entity* player);

#endif