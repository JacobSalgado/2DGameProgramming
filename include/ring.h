#ifndef __COIN_H__
#define __COIN_H__

#include "entity.h"

/* Set up for C function definitions, even when using C++ */
#ifdef __cplusplus
extern "C" 
{
	
}
#endif

typedef struct
{
	Entity* ring_list;
	Uint32	ring_max;
	int ring_sound;
}RingManager;

void ring_system_init(Uint32 max);

void ring_draw(Entity* self);

void ring_system_draw();

void ring_system_close();

/**
* @brief spawn a ring in a specific position
* @return NULL on error, or a pointer to a ring
* @param posX the x-coordinate spawn position
* @param posY the y-coordinate spawn position
*/
Entity* ring_new(float x, float y);

/**
* @brief obtains the rect size of the ring
* @return the ring's collision box
*/
//SDL_Rect ring_rect(Entity* ring);

/**
* @brief the ring to update. Mainly for animation
*/
void ring_update(Entity* ring);

/**
* @brief destroy the ring if you know you will no longer use it
* @param ring the ring to destroy
*/
void ring_destroy(Entity* ring);

/**
* @brief the ring to free/destroy
*/
void ring_free(Entity* ring);

/**
* @brief check if the player is overlapping the ring
* @param ring the ring to collect
* @param player the player collecting the ring
* @return 1 if collecting, 0 otherwise
*/
int ring_collect(Entity* ring, Entity* player);

Uint32 ring_system_get_max();

Entity* ring_system_get(int index);

#endif
