#ifndef __ENEMY_H__
#define __ENEMY_H__

/* Set up for C function definitions, even when using C++ */
#ifdef __cplusplus
extern "C" 
{
	#include "entity.h"
}
#endif

/**
* @brief spawn a player
* @return NULL on error, or a pointer to the player otherwise
*/
/*Entity* enemy_new(Uint16 posX, Uint16 posY);

SDL_Rect enemy_rect(Entity* enemy);*/

class Enemy
{
public:
	Entity* entity; // pointer into existing C pool

	Enemy(int x, int y ); // < constructor
	virtual ~Enemy(); // < deconstructor

	SDL_Rect rect();
	virtual void think();
	virtual void update()/* = 0*/;
	//virtual void draw()/* = 0*/;
};


#endif