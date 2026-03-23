#ifndef __SPEEDPAD_H__
#define __SPEEDPAD_H__

#include "entity.h"

typedef struct
{
	float launch_velocity;
	Uint8 triggered;
	Uint8 direction;
	float cooldown_timer;
}SpeedpadData;

/**
* @brief the location to spawn the spring
* @param x the x-coordinate
* @param y the y-coordinate
*/
Entity* speedpad_new(float x, float y);

void speedpad_think(Entity* self);

void speedpad_update(Entity* self);

void speedpad_free(Entity* self);

/**
* @brief launches the entity rightward after overlapping
* @param speedpad the speedpad to launch the player
* @param player the player being launched
* @return 1 if launching, 0 otherwise
*/
int speedpad_activate(Entity* speedpad, Entity* player);


#endif
