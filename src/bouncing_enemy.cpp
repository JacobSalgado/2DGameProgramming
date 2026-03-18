#include "bouncing_enemy.h"

BouncingEnemy::BouncingEnemy(int x, int y) : Enemy(x, y)
{

}

BouncingEnemy::~BouncingEnemy()
{

}

void BouncingEnemy::think()
{
	Enemy::think(); // calling inherited
}

void BouncingEnemy::update()
{
	Enemy::update(); // calling inherited
}

void BouncingEnemy::onCollision(Player& player)
{

}