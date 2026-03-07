#include "simple_logger.h"

#include "collision.h"
#include "level.h"
#include "entity.h"
//#include "player.h"
//#include "enemy.h"

/*/int check_collision(Entity* player, Entity* enemy)
{
	SDL_Rect playerRect = player_rect(player);
	//SDL_Rect enemyRect = enemy_rect(enemy);
	
	//int result = SDL_HasIntersection(&playerRect, &enemyRect);

	if (result)
	{
		//gf2d_sprite_delete(enemy);
		entity_destroy(enemy);
		slog("COLLISION DETECTED!");
		return result;
	}
}*/

/*/int check_surface_collision(World* level, Entity* player)
{
	int pX = player->position.x, pY = player->position.y, pW = player->width, pH = player->height;

	int tileX = pX / level->tileWidth;
	int tileY = pY / level->tileHeight;

	int index = tileY * level->tileSet->frame_w + tileX;

	if (level->tileMap[index] == 1)
	{
		slog("surface collision detected!");
		return 1;
	}
	return 0;

	/*if (level->tileMap[pX][pY] == 1)
	{
		slog("surface collision detected!");
		return 1;
	}
	slog("no surface collision");
	return 0;
}*/

