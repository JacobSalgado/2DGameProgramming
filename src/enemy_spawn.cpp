#include "enemy_spawn.h"
#include "fly_enemy.h"
#include "turret_enemy.h"

void spawn_fly_enemy(float x, float y)
{
	new FlyEnemy(x, y);
}

void spawn_turret_enemy(float x, float y, Entity* player)
{
	new TurretEnemy(x, y, player);
}