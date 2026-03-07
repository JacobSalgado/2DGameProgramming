#ifndef __ENEMY_H__
#define __ENEMY_H__

/* Set up for C function definitions, even when using C++ */
#ifdef __cplusplus
extern "C" 
{
	#include "entity.h"
}
#endif

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