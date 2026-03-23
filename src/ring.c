#include "simple_logger.h"

#include "gfc_shape.h"

#include "ring.h"

Entity* ring_new(float x, float y)
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

	ring->type = ENTITY_TYPE_RING;
	ring->frame = 0; /*<the current frame of animation for the sprite>*/
	ring->position = gfc_vector2d(x, y);
	ring->velocity = gfc_vector2d(0, 0);

	ring->update = ring_update;
	ring->free = ring_free;
	ring->gravityOn = false;

	return ring;
}

int ring_collect(Entity* ring, Entity* player)
{
	GFC_Shape ring_circle;
	GFC_Shape player_rect;

	if (!ring || !player)
	{
		slog("no ring or player");
		return 0;
	}

	if (!ring->_inuse) return 0;

	float ring_radius = (ring->sprite->frame_w / 2.0f) * 0.5f;

	ring_circle = gfc_shape_circle(ring->position.x, ring->position.y, ring_radius);
	player_rect = gfc_shape_rect(player->position.x, player->position.y, player->sprite->frame_w, player->sprite->frame_h);

	if (gfc_shape_overlap(ring_circle, player_rect))
	{
		slog("player overlapping with ring");
		ring->_inuse = 0;
		entity_destroy(ring);
		return 1;
	}
	return 0;

}

void ring_update(Entity* ring)
{
	if (!ring) return;

	ring->frame += 0;
	if (ring->frame >= 10) ring->frame = 0;
	//ring->velocity.x += 0.1;

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