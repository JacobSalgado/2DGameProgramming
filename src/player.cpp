#include "SDL_events.h"
#include "player.h"


/* Set up for C function definitions, even when using C++ */
#ifdef __cplusplus
extern "C"
{
#include "simple_logger.h"
#include "gfc_vector.h"
#include "camera.h"
#include "collision.h"
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

	entity->gravityOn = true;
}

Player::~Player()
{
	entity_destroy(entity);
}

void Player::think()
{
	//GFC_Vector2D dir = { 0 };

	float jump_force = -10.0f;
	float move_speed = 3.0f;
	float friction = 0.75f;

	if (entity->boost_timer > 0)
	{
		entity->boost_timer -= 0.016f;
		entity->velocity.x = entity->boost_velocity;
		return;
	}

	Uint8 was_moving_last = moving;

	// horizontal movement
	if (input.left)
	{
		entity->scale.x = -fabs(entity->scale.x);
		entity->velocity.x = -move_speed;
		if (!moving)
		{
			entity->sprite->frame_w = 106;
			entity->frame = 25;
		}
		moving = true;
	}
	else if (input.right)
	{
		entity->scale.x = fabs(entity->scale.x);
		entity->velocity.x = move_speed;
		// previous issue is that think is a continuous check so frame would stay still if input is being pressed
		if (!moving) // this helps reset frame when sonic starts to move
		{
			entity->sprite->frame_w = 106;
			entity->frame = 25;
		}
		moving = true;
	}
	else
	{
		entity->sprite->frame_w = 94;
		entity->velocity.x *= friction; // to stop the player
		moving = false;
	}

	// jumping
	if (input.jump && entity->onGround)
	{
		entity->sprite->frame_w = 94;
		jumping = true;
		entity->frame = 9; // p
		entity->velocity.y = jump_force;
		entity->onGround = 0;
		input.jump = false;
	}
}

void Player::update()
{	
	entity->frame += 0.1; // how fast the frame plays
	if (!entity->onGround && jumping)
	{
		if (entity->frame >= 13) entity->frame = 9;
	}
	else if (entity->onGround)
	{
		jumping = false;
		if (!moving)
		{
			entity->frame = 0;
		}
		was_moving = moving;
		if (moving)
		{
			if (entity->frame >= 28) entity->frame = 25;
		}
	}
	
	//if (entity->frame >= 26) entity->frame = 0;
	camera_center_on(entity->position);

	//gfc_vector2d_add(entity->position, entity->position, entity->velocity);
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
		case SDLK_UP: //input.up = true; break; // positive y is downwards
		//case SDLK_DOWN: input.down = true; break;
		case SDLK_SPACE: input.jump = true; break;
		case SDLK_LEFT: input.left = true; break;
		case SDLK_RIGHT: input.right = true; break;
		//case SDLK_r: ring_new(entity); break;
		}
	}
	// slow player down
	else if (event->type == SDL_KEYUP && event->key.repeat == 0)
	{
		switch (event->key.keysym.sym)
		{
		case SDLK_UP:  // positive y is downwards
		case SDLK_SPACE: input.jump = false; break;
		case SDLK_LEFT: input.left = false; break;
		case SDLK_RIGHT: input.right = false; break;
		}
	}
}

extern "C" int player_is_jumping(Entity* player)
{
	if (!player || !player->data) return 0;
	Player* self = (Player*)player->data;
	if (self->jumping)
		return 1;
	else
		return 0;
}

SDL_Rect Player::rect()
{
	SDL_Rect playerRect = { entity->position.x, entity->position.y, 128, 128 };

	return playerRect;
}