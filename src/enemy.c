#include "simple_logger.h"

#include "enemy.h"

void enemy_think(Entity* enemy);
void enemy_update(Entity* enemy);
void enemy_free(Entity* enemy);

Entity* enemy_new()
{
	Entity* enemy;
	enemy = entity_new();

	if (!enemy)
	{
		slog("failed to return an enemy entity");
		return NULL;
	}

	enemy->sprite = gf2d_sprite_load_all
	(
		"images/space_bug.png",
		128,
		128,
		16,
		0
	);
	enemy->frame = 0; /*<the current frame of animation for the sprite*/
	enemy->position = gfc_vector2d(100, 100); /**<the entity's position in the world*/

	enemy->think = enemy_think;
	enemy->update = enemy_update;
	enemy->free = enemy_free;

	return enemy;
}

void enemy_think(Entity* enemy)
{
	GFC_Vector2D dir = { 0 };
	Sint32 mx = 0, my = 0;
	if (!enemy) return;

	gfc_vector2d_scale(enemy->velocity, dir, 3);
}

void enemy_update(Entity* enemy)
{
	if (!enemy) return;
	enemy->frame += 0.1;
	if (enemy->frame >= 16) enemy->frame = 0;

	gfc_vector2d_add(enemy->position, enemy->position, enemy->velocity);
}

void enemy_free(Entity* enemy)
{
	if (!enemy) return;
}



