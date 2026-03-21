#include "ground_enemy.h"

GroundEnemy::GroundEnemy(int x, int y) : Enemy(x, y)
{
	entity->width = 184;
	entity->height = 97;

	entity->scale = gfc_vector2d(0.5f, 0.5f);

	entity->sprite = gf2d_sprite_load_all(
		"images/crab.png",
		184,
		97,
		11,
		0
	);

	entity->gravityOn = true;
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