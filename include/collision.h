#ifndef __COLLISION_H__
#define __COLLISION_H__

#include "SDL.h"
#include "entity.h"

/**
* @brief checks for collision between two rectangles
* @param a first rectangle collision object
* @param b second rectangle collision object
*/
int check_collision(Entity* player, Entity* enemy);

#endif
