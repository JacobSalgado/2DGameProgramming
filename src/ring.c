#include "simple_logger.h"

#include "ring.h"

Entity* ring_new(Entity* player)
{
	//GFC_Color ringColor = gfc_color8();

	Entity* ring;
	ring = entity_new();

	if (!ring)
	{
		slog("failed to return ring");
		return NULL;
	}

	ring->sprite = gf2d_sprite_load_all(
		"images/ring.png",
		64,
		64,
		1,
		0
	);
	ring->frame = 0; /*<the current frame of animation for the sprite>*/
	ring->position = gfc_vector2d(player->position.x, player->position.y);

	ring->update = ring_update;
	ring->free = ring_free;

	return ring;
}

SDL_Rect ring_rect(Entity* ring)
{
	if (!ring) return;
}

void ring_update(Entity* ring)
{
	if (!ring) return;

	ring->frame += 0;
	if (ring->frame >= 10) ring->frame = 0;
	ring->velocity.x += 0.1;

	gfc_vector2d_add(ring->position, ring->position, ring->velocity);
}

void ring_destroy(Entity* ring)
{
	if (!ring) return;
	gf2d_sprite_delete(ring);
}

void ring_free(Entity* ring)
{
	if (!ring) return;
}