#include "simple_logger.h"

#include "collision.h"
#include "player.h"
#include "enemy.h"

int check_collision(Entity* player, Entity* enemy)
{
	SDL_Rect playerRect = player_rect(player);
	SDL_Rect enemyRect = enemy_rect(enemy);
	
	int result = SDL_HasIntersection(&playerRect, &enemyRect);

	if (result)
	{
		//gf2d_sprite_delete(enemy);
		slog("COLLISION DETECTED!");
	}

	return result;
	
}