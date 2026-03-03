#ifndef __ENEMY_H__
#define __ENEMY_H__

#include "entity.h"

/**
* @brief spawn a player
* @return NULL on error, or a pointer to the player otherwise
*/
Entity* enemy_new(Uint16 posX, Uint16 posY);

SDL_Rect enemy_rect(Entity* enemy);


#endif