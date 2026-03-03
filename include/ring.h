#ifndef __COIN_H__
#define __COIN_H__

#include "entity.h"

/**
* @brief spawn a ring in a specific position
* @return NULL on error, or a pointer to a ring
* @param posX the x-coordinate spawn position
* @param posY the y-coordinate spawn position
*/
Entity* ring_new(Entity* player);

/**
* @brief obtains the rect size of the ring
* @return the ring's collision box
*/
SDL_Rect ring_rect(Entity* ring);

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

#endif
