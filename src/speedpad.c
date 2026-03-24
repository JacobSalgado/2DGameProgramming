#include "simple_logger.h"

#include "gfc_shape.h"

#include "entity.h"
#include "speedpad.h"

#define SPEEDPAD_LAUNCH_VELOCITY 15.0f

void speedpad_think(Entity* self)
{
	if (!self) return;

	SpeedpadData* data = (SpeedpadData*)self->data;
	if (!data) return;
}

Entity* speedpad_new(float x, float y)
{
	Entity* speedpad = entity_new();
	if (!speedpad)
	{
		slog("failed to return spring");
		return NULL;
	}

	//speedpad->scale = gfc_vector2d(0.3f, 0.3f);

	speedpad->sprite = gf2d_sprite_load_all(
		"images/arrow.png",
		64,
		64,
		1,
		0);

	SpeedpadData* data = gfc_allocate_array(sizeof(SpeedpadData), 1);
	data->launch_velocity = SPEEDPAD_LAUNCH_VELOCITY;

	speedpad->type = ENTITY_TYPE_SPEEDPAD;
	speedpad->frame = 0; /*<the current frame of animation for the sprite>*/
	speedpad->data = data;
	speedpad->position = gfc_vector2d(x, y);
	speedpad->velocity = gfc_vector2d(0, 0);

	speedpad->think = speedpad_think;
	speedpad->update = speedpad_update;
	speedpad->free = speedpad_free;
	speedpad->gravityOn = false;

	return speedpad;
}

void speedpad_update(Entity* self)
{
	if (!self) return;
}

void speedpad_free(Entity* self)
{
	if (!self) return;
}

int speedpad_activate(Entity* speedpad, Entity* player)
{
	GFC_Shape speedpad_rect;
	GFC_Shape player_rect;
	SpeedpadData* data;

	if (!speedpad || !player) return 0;
	if (!speedpad->_inuse) return 0;

	data = (SpeedpadData*)speedpad->data;

	if (data->cooldown_timer > 0)
	{
		data->cooldown_timer -= 0.016f;
		return 0;
	}

	speedpad_rect = gfc_shape_rect(speedpad->position.x, speedpad->position.y, speedpad->sprite->frame_w, speedpad->sprite->frame_h);
	player_rect = gfc_shape_rect(player->position.x, player->position.y, player->sprite->frame_w, player->sprite->frame_h);

	if (gfc_shape_overlap(speedpad_rect, player_rect))
	{
		slog("OVERLAP ON SPEED BOOST - speeding player up");
		data->direction = 1;
		player->boost_timer = 0.5f;
		player->boost_velocity = data->launch_velocity * data->direction;
		//slog("player velocity before: %.2f", player->velocity.x);
		//player->velocity.x = data->launch_velocity * data->direction;
		//slog("player velocity before: %.2f", player->velocity.x);
		data->triggered = 1;
		data->cooldown_timer = 0.5f;
		//speedpad->frame = ;
		return 1;
	}
	return 0;
}