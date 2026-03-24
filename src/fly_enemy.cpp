#include <SDL.h>

#include "simple_logger.h"
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

	entity->gravityOn = false;

	patrolTime = SDL_GetTicks();
	turnAround = rand() % (6001 + 2000);
	patrolDistance = 20;
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
	Uint32 currentTime = SDL_GetTicks();

	float move_speed = 2.0f;

	entity->velocity.x = move_speed * direction;
	//slog("X position:", entity->position.x);

	//slog("FlyEnemy velocity.x: %f, direction: %f, position.x: %f",
		//entity->velocity.x, direction, entity->position.x);

	if (currentTime - patrolTime >= turnAround)
	{
		direction *= -1.0f;
		entity->scale.x = -fabs(entity->scale.x);
		patrolTime = SDL_GetTicks();
		turnAround = rand() % (4001 + 2000); // 2 - 6 seconds
	}
}

/*void BouncingEnemy::onCollision(Player& player)
{

}*/

