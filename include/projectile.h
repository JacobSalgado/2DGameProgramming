#ifndef __PROJECTILE_H__
#define __PROJECTILE_H__

#include "entity.h"

typedef struct {
	float lifetime;
	float timer;
	int damage;
}ProjectileData;

/**
* @brief creating a new projectile
* @param position the x,y coordinates to spawn projectile
* @param direction the direction to fire projectile
* @param speed the speed of the projectile
*/
Entity* projectile_new(GFC_Vector2D position, GFC_Vector2D direction, float speed);

#endif
