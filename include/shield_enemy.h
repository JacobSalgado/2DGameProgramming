#ifndef __SHIELD_ENEMY_H__
#define __SHIELD_ENEMY_H__

#include "enemy.h"
//#include "player.h"

class ShieldEnemy : public Enemy
{
public:
	ShieldEnemy(int x, int y);		/// < constructor
	~ShieldEnemy();				/// < deconstructor

	void think() override;
	void update() override;
	//void onCollision(Player& player);
};

#endif
