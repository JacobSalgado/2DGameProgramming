#ifndef __BOUNCING_ENEMY_H__
#define __BOUNCING_ENEMY_H__

#include "enemy.h"
//#include "player.h"

class BouncingEnemy : public Enemy
{
public:
	BouncingEnemy(int x, int y);		/// < constructor
	~BouncingEnemy();				/// < deconstructor

	void think() override;
	void update() override;
	//void onCollision(Player& player);
};

#endif
