#ifndef __FLY_ENEMY_H__
#define __FLY_ENEMY_H__

#include "enemy.h"
//#include "player.h"

class FlyEnemy : public Enemy
{
public:
	FlyEnemy(int x, int y);		/// < constructor
	~FlyEnemy();				/// < deconstructor

	void think() override;
	void update() override;
	//void onCollision(Player& player);

private:
	// movement
	Uint32 patrolTime;
	Uint32 turnAround;
	int patrolDistance;
	float direction = 1.0f; // 1 = right, -1 = left
};

#endif
