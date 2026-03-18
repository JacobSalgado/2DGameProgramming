#include "turret_enemy.h"

TurretEnemy::TurretEnemy(int x, int y) : Enemy(x, y)
{

}

TurretEnemy::~TurretEnemy()
{

}

void TurretEnemy::think()
{
	Enemy::think(); // calling inherited
}

void TurretEnemy::update()
{
	Enemy::update(); // calling inherited
}

void TurretEnemy::onCollision(Player& player)
{

}