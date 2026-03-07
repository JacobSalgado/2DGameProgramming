#ifndef __COLLISION_H__
#define __COLLISION_H__

#include "SDL.h"
#include "level.h"
#include "entity.h"

/**
* @brief checks for collision between two rectangles
* @param a first rectangle collision object
* @param b second rectangle collision object
*/
int check_collision(Entity* player, Entity* enemy);

int check_surface_collision(World* world, Entity* player);

#endif
