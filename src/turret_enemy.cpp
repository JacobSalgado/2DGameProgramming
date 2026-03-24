#include "simple_logger.h"
#include "turret_enemy.h"

#ifdef __cplusplus
extern "C"
{
#include "projectile.h"
}
#endif

TurretEnemy::TurretEnemy(int x, int y, Entity* player) : Enemy(x, y)
{
	target = player;
	fireRate = 2.0f;
	fireTimer = 0.0f;
	fireRange = 300.0f;
	projectileSpeed = 20.0f;

	entity->width = 128;
	entity->height = 128;

	entity->sprite = gf2d_sprite_load_all(
		"images/ed210.png",
		128,
		128,
		16,
		0
	);

	entity->gravityOn = true;
}

TurretEnemy::~TurretEnemy()
{

}

void TurretEnemy::think()
{
	Enemy::think(); // calling inherited

	if (!target) return;

	fireTimer += 0.016f;

	if (distanceToTarget() <= fireRange && fireTimer >= fireRate)
	{
		fireProjectile();
		fireTimer = 0.0f;
	}
}

void TurretEnemy::update()
{
	//Enemy::update(); // calling inherited
}

void TurretEnemy::fireProjectile()
{
	GFC_Vector2D direction = directionToTarget();
	GFC_Vector2D spawn = gfc_vector2d((entity->position.x + direction.x * 32.0f), (entity->position.y + direction.y * 32.0f));
	projectile_new(spawn, direction, projectileSpeed);
}

GFC_Vector2D TurretEnemy::directionToTarget()
{
	GFC_Vector2D direction;
	direction.x = target->position.x - entity->position.x;
	direction.y = target->position.y - entity->position.y;

	float length = sqrt(direction.x * direction.x + direction.y * direction.y);
	if (length > 0) 
	{
		direction.x = direction.x / length;
		direction.y = direction.y / length;
	}
	return direction;
}

float TurretEnemy::distanceToTarget()
{
	float dx = target->position.x - entity->position.x;
	float dy = target->position.y - entity->position.y;
	return sqrt(dx * dx + dy * dy);
}