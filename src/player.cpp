#include "SDL_events.h"
#include "player.h"


/* Set up for C function definitions, even when using C++ */
#ifdef __cplusplus
extern "C"
{
#include "simple_logger.h"
#include "gfc_vector.h"
#include "camera.h"
#include "ring.h"
}
#endif

Player* Player::_instance = nullptr;

Player* Player::create_instance(int x, int y)
{
	if (!_instance)
	{
		_instance = new Player(x, y);
	}
	return _instance;
}

void Player::destroy_instance()
{
	if (_instance)
	{
		delete _instance;
		_instance = nullptr;
	}
}

Player::Player(int x, int y)
{
	entity = entity_new();

	entity->type = ENTITY_TYPE_PLAYER;

	entity->position.x = x;
	entity->position.y = y;

	/* TEMPORARY */
	entity->width = 94;
	entity->height = 130;
	entity->velocity = gfc_vector2d(0, 0);

	/* TEMPORARY */
	entity->sprite = gf2d_sprite_load_all(
		"images/sonic.png",
		94,
		130,
		13,
		0
	);
	entity->frame = 0;
	entity->data = this; // important to set so it doesn't crash

	entity->think = [](Entity* e)
		{
			Player* self = (Player*)e->data;
			self->think();
		};

	entity->update = [](Entity* e)
		{
			Player* self = (Player*)e->data;
			self->update();
		};
}

Player::~Player()
{
	entity_destroy(entity);
}

void Player::think()
{
	GFC_Vector2D dir = { 0 };
}

void Player::update()
{
	entity->frame += 0.2; // how fast the frame plays
	if (entity->frame >= 13) entity->frame = 0;

	gfc_vector2d_add(entity->position, entity->position, entity->velocity);

	camera_center_on(entity->position);
}

void Player::handle_input(SDL_Event* event)
{
	int movementVelocity = 5;
	int slowDown = 4;

	if (event->type == SDL_KEYDOWN && event->key.repeat == 0)
	{
		// SWITCH VELOCITY
		switch (event->key.keysym.sym)
		{
		case SDLK_UP:
			entity->velocity.y -= movementVelocity; break; // positive y is downwards
		case SDLK_DOWN:
			entity->velocity.y += movementVelocity; break;
		case SDLK_LEFT:
			entity->velocity.x -= movementVelocity; break;
		case SDLK_RIGHT:
			entity->velocity.x += movementVelocity; break;
		case SDLK_SPACE:
			ring_new(entity);
		}
	}
	// slow player down
	else if (event->type == SDL_KEYUP && event->key.repeat == 0)
	{
		switch (event->key.keysym.sym)
		{
		case SDLK_UP:
			entity->velocity.y += slowDown; break; // positive y is downwards
		case SDLK_DOWN:
			entity->velocity.y -= slowDown; break;
		case SDLK_LEFT:
			entity->velocity.x += slowDown; break;
		case SDLK_RIGHT:
			entity->velocity.x -= slowDown; break;
		}
	}
}

SDL_Rect Player::rect()
{
	SDL_Rect playerRect = { entity->position.x, entity->position.y, 128, 128 };

	return playerRect;
}