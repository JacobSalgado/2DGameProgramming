#ifndef __TURRET_ENEMY_H__
#define __TURRET_ENEMY_H__

#include "enemy.h"
#include "player.h"

class TurretEnemy : public Enemy
{
public:
	TurretEnemy(int x, int y);		/// < constructor
	~TurretEnemy();				/// < deconstructor

	void think() override;
	void update() override;
	void onCollision(Player& player);
};

#endif

