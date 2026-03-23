#ifndef __COLLISION_H__
#define __COLLISION_H__

#include "SDL.h"
#include "world.h"
#include "entity.h"

/* Set up for C function definitions, even when using C++ */
#ifdef __cplusplus
extern "C" 
{
	#include "player.h"
}
#endif

/**
* @brief checks for collision between two rectangles
* @param a first rectangle collision object
* @param b second rectangle collision object
*/
//int check_collision(Entity* player, Entity* enemy);

//int check_surface_collision(World* world, Entity* player);

void resolveTileCollisionX(Entity* entity, World* world);

void resolveTileCollisionY(Entity* entity, World* world);

Bool isTileSolid(World* world, int tX, int tY);

Bool check_entity_overlap(Entity* a, Entity* b);

void check_spring_collision(Entity* player);

#endif
