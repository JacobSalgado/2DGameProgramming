#include "SDL_events.h"

#include "simple_logger.h"

#include "camera.h"
#include "player.h"
#include "ring.h"

void player_think(Entity* self);
void player_update(Entity* self);
void player_free(Entity* self);

int movementVelocity = 5;
int slowDown = 4;

Entity *player_new()
{
	Entity *self;
	self = entity_new();

	if (!self)
	{
		slog("failed to spawn a player entity");
		return NULL;
	}

	self->type = ENTITY_TYPE_PLAYER;
	self->sprite = gf2d_sprite_load_all(
		"images/sonic.png",
		94,
		130,
		13,
		0
	);
	self->frame = 0; /*<the current frame of animation for the sprite*/
	self->position = gfc_vector2d(100, 100); /**<the entity's position in the world*/
	self->velocity = gfc_vector2d(0, 0);
	self->width = 94;
	self->height = 130;
	

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
	
	/*/SDL_GetMouseState(&mx, &my);
	if (self->position.x < mx)dir.x = 1;
	if (self->position.y < my)dir.y = 1;
	if (self->position.x > mx)dir.x = -1;
	if (self->position.y > my)dir.y = -1;
	gfc_vector2d_normalize(&dir);

	gfc_vector2d_scale(self->velocity, dir, 3);*/
}
void player_update(Entity* self)
{
	if (!self) return;
	self->frame += 0.2; // how fast the frame plays
	if (self->frame >= 13) self->frame = 0;

	gfc_vector2d_add(self->position,self->position,  self->velocity);

	camera_center_on(self->position);
}
void player_free(Entity* self)
{
	if (!self) return;
}

void player_input(Entity* self, SDL_Event* event)
{
	if (!self || !event) return;

	if (event->type == SDL_KEYDOWN && event->key.repeat == 0)
	{
		// SWITCH VELOCITY
		switch (event->key.keysym.sym)
		{
		case SDLK_UP:
			self->velocity.y -= movementVelocity; break; // positive y is downwards
		case SDLK_DOWN:
			self->velocity.y += movementVelocity; break;
		case SDLK_LEFT:
			self->velocity.x -= movementVelocity; break;
		case SDLK_RIGHT:
			self->velocity.x += movementVelocity; break;
		case SDLK_SPACE:
			ring_new(self);
		}
	}
	// slow player down
	else if (event->type == SDL_KEYUP && event->key.repeat == 0)
	{
		switch (event->key.keysym.sym)
		{
		case SDLK_UP:
			self->velocity.y += slowDown; break; // positive y is downwards
		case SDLK_DOWN:
			self->velocity.y -= slowDown; break;
		case SDLK_LEFT:
			self->velocity.x += slowDown; break;
		case SDLK_RIGHT:
			self->velocity.x -= slowDown; break;
		}
	}
}

SDL_Rect player_rect(Entity* player)
{
	if (!player) return;
	SDL_Rect playerRect = { player->position.x, player->position.y, 128, 128 };

	return playerRect;
}