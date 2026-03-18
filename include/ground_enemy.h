#ifndef __GROUND_ENEMY_H__
#define __GROUND_ENEMY_H__

#include "enemy.h"
#include "player.h"

class GroundEnemy : public Enemy
{
public:
	GroundEnemy(int x, int y);		/// < constructor
	~GroundEnemy();				/// < deconstructor

	void think() override;
	void update() override;
	void onCollision(Player& player);
};

#endif
