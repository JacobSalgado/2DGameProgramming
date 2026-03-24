#ifndef __TURRET_ENEMY_H__
#define __TURRET_ENEMY_H__

#ifdef __cplusplus
extern "C"
{
#include "gf2d_sprite.h"
#include "gfc_vector.h"
}
#endif

#include "enemy.h"
//#include "player.h"

class TurretEnemy : public Enemy
{
public:
	TurretEnemy(int x, int y, Entity* player);		/// < constructor
	~TurretEnemy();				/// < deconstructor

	void think() override;
	void update() override;
	//void onCollision(Player& player);
private:
	Entity* target;
	float fireRate;
	float fireTimer;
	float fireRange;
	float projectileSpeed;

	void fireProjectile();
	float distanceToTarget();
	GFC_Vector2D directionToTarget();
};

#endif

