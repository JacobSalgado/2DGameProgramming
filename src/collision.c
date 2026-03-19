#include "simple_logger.h"

#include "collision.h"
#include "world.h"
#include "entity.h"
//#include "player.h"
//#include "enemy.h"

void resolveTileCollisionX(Entity* entity, World* world)
{
	int left = (int)(entity->position.x) / world->tileSet->frame_w;
	int right = (int)(entity->position.x + entity->width - 1) / world->tileSet->frame_w;
	int top = (int)(entity->position.y) / world->tileSet->frame_h;
	int bottom = (int)(entity->position.y + entity->height - 2) / world->tileSet->frame_h;

	for (int tY = top; tY <= bottom; tY++)
	{
		for (int tX = left; tX <= right; tX++)
		{
			if (!isTileSolid(world, tX, tY)) continue;

			float tileLeft = (float)(tX * world->tileSet->frame_w);
			float tileRight = tileLeft + world->tileSet->frame_w;

			if (entity->velocity.x > 0 /*&& entity->position.x + entity->width > tileLeft && entity->position.x < tileRight*/ )
			{
				entity->position.x = tileLeft - entity->width;
				entity->velocity.x = 0;
			}
			else if (entity->velocity.x < 0 /*&& entity->position.x < tileRight*/ )
			{
				entity->position.x = tileRight;
				entity->velocity.x = 0;
			}
		}
	}
}

void resolveTileCollisionY(Entity* entity, World* world)
{
	int left = (int)(entity->position.x + 1) / world->tileSet->frame_w;
	int right = (int)(entity->position.x + entity->width - 2) / world->tileSet->frame_w;
	int top = (int)(entity->position.y) / world->tileSet->frame_h;
	int bottom = (int)(entity->position.y + entity->height - 1) / world->tileSet->frame_h;

	entity->onGround = 0;

	/*slog("--- Y Collision ---");
	slog("Position: %.2f, %.2f\n", entity->position.x, entity->position.y);
	slog("Velocity: %.2f, %.2f\n", entity->velocity.x, entity->velocity.y);
	slog("Tile bounds: L%d R%d T%d B%d\n", left, right, top, bottom);*/
	//slog("onGround: %d\n", entity->onGround);

	for (int tY = top; tY <= bottom; tY++)
	{
		for (int tX = left; tX <= right; tX++)
		{
			if (!isTileSolid(world, tX, tY)) continue;

			float tileTop = tY * world->tileSet->frame_h;
			float tileBottom = tileTop + world->tileSet->frame_h;

			//slog("Hit solid tile [%d, %d] tileTop:$.2f tileBottom:%.2f\n",
			//	tX, tY, tileTop, tileBottom);
			//slog("Player bottom: %.2f playerTop: %.2f\n",
			//	entity->position.y + (float)entity->height, entity->position.y);

			if (entity->velocity.y > 0 && (entity->position.y + entity->height) > tileTop && (entity->position.y) < tileTop)
			{
				slog("FLOOR HIT - snapping to %.2f\n", tileTop - (float)entity->height);
				entity->position.y = tileTop - entity->height;
				entity->velocity.y = 0;
				entity->onGround = 1;
			}
			else if (entity->velocity.y < 0 && entity->position.y < tileBottom && (entity->position.y + entity->height) > tileBottom)
			{
				slog("CEILING HIT - snapping to %.2f\n", tileBottom);
				entity->position.y = tileBottom;
				entity->velocity.y = 0;
			}
		}
	}
}

Bool isTileSolid(World* world, int tX, int tY)
{
	if (tX < 0 || tY < 0 || tX >= world->tileWidth || tY >= world->tileHeight)
		return 1;

	return (world->tileMap[tY * world->tileWidth + tX] != 0) ? 1 : 0;
}

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

