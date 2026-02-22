#include "SDL_events.h"

#include "simple_logger.h"

#include "player.h"

void player_think(Entity* self);
void player_update(Entity* self);
void player_free(Entity* self);

Entity *player_new()
{
	Entity *self;
	self = entity_new();

	if (!self)
	{
		slog("failed to spawn a player entity");
		return NULL;
	}

	self->sprite = gf2d_sprite_load_all(
		"images/ed210.png",
		128,
		128,
		16,
		0
	);
	self->frame = 0; /*<the current frame of animation for the sprite*/
	self->position = gfc_vector2d(0, 0); /**<the entity's position in the world*/
	

	self->think = player_think;
	self->update = player_update;
	self->free = player_free;

	return self;
}

void player_think(Entity* self)
{
	GFC_Vector2D dir = { 0 };
	Sint32 mx = 0, my = 0;
	if (!self) return;
	
	/*SDL_GetMouseState(&mx, &my);
	if (self->position.x < mx)dir.x = 1;
	if (self->position.y < my)dir.y = 1;
	if (self->position.x > mx)dir.x = -1;
	if (self->position.y > my)dir.y = -1;
	gfc_vector2d_normalize(&dir);*/

	gfc_vector2d_scale(self->velocity, dir, 3);
}
void player_update(Entity* self)
{
	if (!self) return;
	self->frame += 0.1;
	if (self->frame >= 16) self->frame = 0;

	gfc_vector2d_add(self->position,self->position,  self->velocity);
}
void player_free(Entity* self)
{
	if (!self) return;
}

void player_input(Entity* self, SDL_Event* event)
{
	if (!self || !event) return;

	if (event->type == SDL_KEYDOWN)
	{
		switch (event->key.keysym.sym)
		{
			case SDLK_w:
				self->position.y -= 10; // positive y is downwards
				slog("w key pressed!");
				break;
			case SDLK_a:
				self->position.x -= 10;
				break;
			case SDLK_s:
				self->position.y += 10;
				break;
			case SDLK_d:
				self->position.x += 10;
				break;
		}
	}
}