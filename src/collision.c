#include "simple_logger.h"

#include "collision.h"
#include "world.h"
#include "entity.h"
//#include "player.h"
//#include "enemy.h"

void resolveTileCollisionX(Entity* entity, World* world)
{
	int left = (int)(entity->position.x + 1) / world->tileSet->frame_w;
	int right = (int)(entity->position.x + entity->width - 1) / world->tileSet->frame_w;
	int top = (int)(entity->position.y + 1) / world->tileSet->frame_h;
	int bottom = (int)(entity->position.y + entity->height - 1) / world->tileSet->frame_h;

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
	int right = (int)(entity->position.x + entity->width - 1) / world->tileSet->frame_w;
	int top = (int)(entity->position.y + 1) / world->tileSet->frame_h;
	int bottom = (int)(entity->position.y + entity->height - 1) / world->tileSet->frame_h;

	entity->onGround = 0;

	for (int tY = top; tY <= bottom; tY++)
	{
		for (int tX = left; tX <= right; tX++)
		{
			if (!isTileSolid(world, tX, tY)) continue;

			float tileTop = tY * world->tileSet->frame_h;
			float tileBottom = tileTop + world->tileSet->frame_h;

			if (entity->velocity.y > 0 && (entity->position.y + entity->height) > tileTop && (entity->position.y) < tileTop)
			{
				//slog("FLOOR HIT - snapping to %.2f\n", tileTop - (float)entity->height);
				entity->position.y = tileTop - entity->height;
				entity->velocity.y = 0;
				entity->onGround = 1;
			}
			else if (entity->velocity.y < 0 && entity->position.y < tileBottom && (entity->position.y + entity->height) > tileBottom)
			{
				//slog("CEILING HIT - snapping to %.2f\n", tileBottom);
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

Bool check_entity_overlap(Entity* a, Entity* b)
{
	if (!a || !b) return 0;
}

