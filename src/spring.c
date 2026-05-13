#include "simple_logger.h"

#include "gfc_shape.h"

#include "entity.h"
#include "audio.h"
#include "spring.h"

#define SPRING_LAUNCH_VELOCITY -12.0f

void spring_think(Entity* self)
{
	if (!self) return;

	SpringData* data = (SpringData*)self->data;
	if (!data) return;
}

Entity* spring_new(float x, float y)
{
	Entity* spring = entity_new();
	if (!spring)
	{
		slog("failed to return spring");
		return NULL;
	}

	spring->sprite = gf2d_sprite_load_all(
		"images/spring.png",
		114,
		158,
		2,
		0
	);

	SpringData* data = gfc_allocate_array(sizeof(SpringData), 1);
	data->launch_velocity = SPRING_LAUNCH_VELOCITY;
	data->spring_sound = audio_load_sound("audio/spring.wav");

	spring->type = ENTITY_TYPE_SPRING;
	spring->frame = 0; /*<the current frame of animation for the sprite>*/
	spring->data = data;
	spring->position = gfc_vector2d(x, y);
	spring->velocity = gfc_vector2d(0, 0);

	spring->think = spring_think;
	spring->update = spring_update;
	spring->free = spring_free;
	spring->gravityOn = false;

	return spring;
}

void spring_update(Entity* self)
{
	if (!self) return;
}

void spring_free(Entity* self)
{
	if (!self) return;
}

int spring_activate(Entity* spring, Entity* player)
{
	GFC_Shape spring_rect;
	GFC_Shape player_rect;

	if (!spring || !player) return 0;
	if (!spring->_inuse) return 0;

	spring->frame = 0;

	spring_rect = gfc_shape_rect(spring->position.x, spring->position.y, spring->sprite->frame_w, spring->sprite->frame_h);
	player_rect = gfc_shape_rect(player->position.x, player->position.y, player->sprite->frame_w, player->sprite->frame_h);

	if (gfc_shape_overlap(spring_rect, player_rect))
	{
		SpringData* data = (SpringData*)spring->data;
		player->velocity.y = data->launch_velocity;
		player->onGround = 0;

		data->triggered = 1;
		audio_play_sound(data->spring_sound);
		spring->frame = 1;
		return 1;
	}
	return 0;
}