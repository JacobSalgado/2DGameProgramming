#include "enemy.h"

/* Set up for C function definitions, even when using C++ */
#ifdef __cplusplus
extern "C"
{
	#include "simple_logger.h"

}
#endif

Enemy::Enemy(int x, int y)
{
	entity = entity_new();

	entity->type = ENTITY_TYPE_ENEMY;

	entity->position.x = x;
	entity->position.y = y;

	/* TEMPORARY */
	entity->width = 128;
	entity->height = 128;

	/* TEMPORARY */
	entity->sprite = gf2d_sprite_load_all(
		"images/space_bug.png",
		128,
		128,
		16,
		0
	);
	entity->frame = 0;
	entity->data = this; // important to set so it doesn't crash

	entity->think = [](Entity* e)
		{
			Enemy* self = (Enemy*)e->data;
			self->think();
		};

	entity->update = [](Entity* e)
		{
			Enemy* self = (Enemy*)e->data;
			self->update();
		};
}

Enemy::~Enemy()
{
	entity_destroy(entity);
}

void Enemy::think()
{
	GFC_Vector2D dir = { 0 };

	gfc_vector2d_scale(entity->velocity, dir, 3);
}

void Enemy::update()
{
	entity->frame += 0.1;
	if (entity->frame >= 16) entity->frame = 0;

	gfc_vector2d_add(entity->position, entity->position, entity->velocity);
}

SDL_Rect Enemy::rect()
{
	SDL_Rect enemyRect = { entity->position.x, entity->position.y, 128, 128 };
	return enemyRect;
}