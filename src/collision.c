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

void resolveTerrainCollisionX(Entity* entity, World* world)
{
	if (!world->platforms) return;

	for (int i = 0; i < (int)world->platformCount; i++)
	{
		Platform* p =  &world->platforms[i];

		if (p->type == TERRAIN_ONEWAY) continue;

		float pLeft = p->bounds.x;
		float pRight = p->bounds.x + p->bounds.w;
		float pTop = p->bounds.y;
		float pBottom = p->bounds.y + p->bounds.h;

		if (entity->position.y + entity->height <= pTop || entity->position.y >= pBottom) continue;

		if (entity->position.x > 0 && entity->position.x + entity->width > pLeft && entity->position.x < pLeft)
		{
			entity->position.x = pLeft - entity->width;
			entity->velocity.x = 0;
		}
		else if (entity->velocity.x < 0 && entity->position.x < pRight && entity->position.x + entity->width > pRight)
		{
			entity->position.x = pRight;
			entity->velocity.x = 0;
		}
	}
}

void resolveTerrainCollisionY(Entity* entity, World* world)
{
	if (!world->platforms) return;

	for (int i = 0; i < (int)world->platformCount; i++)
	{
		Platform* p = &world->platforms[i];

		float pLeft = p->bounds.x;
		float pRight = p->bounds.x + p->bounds.w;
		float pTop = p->bounds.y;
		float pBottom = p->bounds.y + p->bounds.h;

		if (entity->position.x + entity->width <= pLeft || entity->position.x >= pRight)  continue;

		if (p->type == TERRAIN_SOLID)
		{
			// land on top of the terrain
			if (entity->velocity.y > 0 && entity->position.y + entity->height > pTop && entity->position.y < pTop)
			{
				entity->position.y = pTop - entity->height;
				entity->velocity.y = 0;
				entity->onGround = 1;
			}
			// ceiling collisionss
			else if (entity->velocity.y < 0 && entity->position.y < pBottom && entity->position.y + entity->height > pBottom)
			{
				entity->position.y = pBottom;
				entity->velocity.y = 0;
			}
		}
		else if (p->type == TERRAIN_ONEWAY)
		{
			float previousBottom = (entity->position.y + entity->height) - entity->velocity.y;
			if (entity->velocity.y > 0 && previousBottom <= pTop && entity->position.y + entity->height >= pTop)
			{
				entity->position.y = pTop - entity->height;
				entity->velocity.y = 0;
				entity->onGround = 1;
			}
		}
		else if (p->type == TERRAIN_MOVING)
		{
			float previousBottom = (entity->position.y + entity->height) - entity->velocity.y;
			if (entity->velocity.y >= 0 && previousBottom <= pTop && entity->position.y + entity->height >= pTop)
			{
				entity->position.y = pTop - entity->height;
				entity->velocity.y = 0;
				entity->onGround = 1;

				// carry sonic
				entity->position.x += p->currentVelocity.x;
				entity->position.y += p->currentVelocity.y;
			}
		}
	}
}

void resolveSlopeCollision(Entity* entity, World* world)
{
	if (!world->slopes) return;

	float footX = entity->position.x + entity->width * 0.5f;
	float footY = entity->position.y + entity->height;

	for (int i = 0; i < (int)world->slopeCount; i++)
	{
		Slope* s = &world->slopes[i];

		float sx1 = s->p1.x, sy1 = s->p1.y;
		float sx2 = s->p2.x, sy2 = s->p2.y;

		float minX, maxX;

		if (sx1 < sx2)
		{
			minX = sx1;
			maxX = sx2;
		}
		else
		{
			minX = sx2;
			maxX = sx1;
		}

		if (footX < minX || footX > maxX) continue;

		if (sx2 == sx1) continue;

		// check to see if sonic is close enough to land on the slope surface
		float t = (footX - sx1) / (sx2 - sx1);
		float slopeY = sy1 + t * (sy2 - sy1);

		float snapRange = s->thickness + fabsf(entity->velocity.y) + 1.0f;

		if (footY >= slopeY && footY <= slopeY + snapRange && entity->velocity.y >= 0)
		{
			// snaps player to slope
			entity->position.y = slopeY - entity->height;
			entity->velocity.y = 0;
			entity->onGround = 1;
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

