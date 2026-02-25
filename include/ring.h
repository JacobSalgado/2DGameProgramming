#ifndef __COIN_H__
#define __COIN_H__

#include "entity.h"

/**
* @brief spawn a ring
* @return NULL on error, or a pointer to a ring
*/
Entity* ring_new();

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
* @brief the ring to free/destroy
*/
void ring_free(Entity* ring);

#endif
