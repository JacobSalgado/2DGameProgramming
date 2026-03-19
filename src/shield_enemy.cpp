#include "shield_enemy.h"

ShieldEnemy::ShieldEnemy(int x, int y) : Enemy(x, y)
{

}

ShieldEnemy::~ShieldEnemy()
{

}

void ShieldEnemy::think()
{
	Enemy::think(); // calling inherited
}

void ShieldEnemy::update()
{
	Enemy::update(); // calling inherited
}

/*void BouncingEnemy::onCollision(Player& player)
{

}*/