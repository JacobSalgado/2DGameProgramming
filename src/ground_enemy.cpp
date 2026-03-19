#include "ground_enemy.h"

GroundEnemy::GroundEnemy(int x, int y) : Enemy(x, y)
{

}

GroundEnemy::~GroundEnemy()
{

}

void GroundEnemy::think()
{
	Enemy::think(); // calling inherited
}

void GroundEnemy::update()
{
	Enemy::update(); // calling inherited
}

/*void BouncingEnemy::onCollision(Player& player)
{

}*/