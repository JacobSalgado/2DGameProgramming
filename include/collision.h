#ifndef __COLLISION_H__
#define __COLLISION_H__

#include "SDL.h"

/**
* @brief checks for collision between two rectangles
* @param a first rectangle collision object
* @param b second rectangle collision object
*/
int collision_check(SDL_Rect a, SDL_Rect b);

#endif
