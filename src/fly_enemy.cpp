#include "fly_enemy.h"

FlyEnemy::FlyEnemy(int x, int y) : Enemy (x,y)
{
	entity->width = 128;
	entity->height = 128;

	entity->sprite = gf2d_sprite_load_all(
		"images/space_bug.png",
		128,
		128,
		16,
		0
	);
}

FlyEnemy::~FlyEnemy()
{

}

void FlyEnemy:: think()
{
	Enemy::think(); // calling inherited
}

void FlyEnemy:: update()  
{
	Enemy::update(); // calling inherited
}

void FlyEnemy::onCollision(Player& player)
{

}

