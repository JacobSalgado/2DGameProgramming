#include "simple_logger.h"

#include "collision.h"

int collision_check(SDL_Rect a, SDL_Rect b)
{
	return SDL_HasIntersection(&a, &b);
}