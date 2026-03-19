#include "simple_logger.h"

#include "camera.h"
#include "world.h"
#include "entity.h"
#include "collision.h"

typedef struct
{
	Entity *entity_list;
	Uint32	entity_max;
}EntityManager;

void entity_system_close();

static EntityManager _entity_manager = { 0 }; /**<initialize a LOCAL global entity manager*/
static World* _active_world = NULL;

void entity_system_initialize(Uint32 max)
{
	if (_entity_manager.entity_list)
	{
		slog("cannot have two instances of an entity manager, one is already active");
		return;
	}

	if (!max)
	{
		slog("cannot allocate 0 entities!");
		return;
	}
	_entity_manager.entity_list = gfc_allocate_array(sizeof(Entity), max);
	if (!_entity_manager.entity_list)
	{
		slog("failed to allocate global entity list");
		return;
	}
	_entity_manager.entity_max = max;
	slog("entity system intialized");
	atexit(entity_system_close);
}

void entity_system_close()
{
	entity_clear_all(NULL);
	if (_entity_manager.entity_list)free(_entity_manager.entity_list);
	memset(&_entity_manager, 0, sizeof(EntityManager));
}

void entity_clear_all(Entity* ignore)
{
	int i;
	for (i = 0; i < _entity_manager.entity_max; i++)
	{
		if (&_entity_manager.entity_list[i] == ignore) continue;
		if (!_entity_manager.entity_list[i]._inuse) continue; // skip this iteration of the loop
		entity_free(&_entity_manager.entity_list[i]);
	}
}

Entity* entity_new()
{
	int i;
	if (!_entity_manager.entity_list)
	{
		slog("entity manager not initialized");
		return NULL;
	}

	for (i = 0; i < _entity_manager.entity_max; i++)
	{
		if (_entity_manager.entity_list[i]._inuse) continue; // skip any active entities
		memset(&_entity_manager.entity_list[i], 0, sizeof(Entity));
		_entity_manager.entity_list[i]._inuse = 1;
		_entity_manager.entity_list[i].scale = gfc_vector2d(1, 1); // default scale
		// setting the default color
		//_entity_manager.entity_list[i].scale = gfc_vector2d(1,1);
		return &_entity_manager.entity_list[i];
	}
	slog("no more available entities");
	return NULL;
}

void entity_free(Entity* self)
{
	if (!self) return; // can't do work on a pointer that doesn't exist
	gf2d_sprite_free(self->sprite);
	// anything else you allocate for an entity would get cleaned up here
	if (self->free)self->free(self->data);
}

void entity_destroy(Entity* self)
{
	entity_free(self);
	entity_manager_remove(self);
}

void entity_manager_remove(Entity* self)
{
	int i;
	for (i = 0; i < _entity_manager.entity_max; i++)
	{
		if (&_entity_manager.entity_list[i] == self)
		{
			_entity_manager.entity_list[i]._inuse = 0;
			return;
		}
	}
}

void entity_check_collisions(Entity* player)
{
	int i;
	for (i = 0; i < _entity_manager.entity_max; i++)
	{
		if (!_entity_manager.entity_list[i]._inuse) continue; // if the entity is gone, SKIP
		//if (&_entity_manager.entity_list[i] == player) continue;
		if (_entity_manager.entity_list[i].type != ENTITY_TYPE_ENEMY) continue; 
		//check_collision(player, &_entity_manager.entity_list[i]);
	}
}

/*void entity_surface_collision(World* world, Entity* player)
{
	check_surface_collision(world, player);
}*/

void entity_think(Entity* self)
{
	if (!self) return;
	// any boilerplate think stuff here
	if (self->think)self->think(self);
}

void entity_system_think()
{
	int i;
	for (i = 0; i < _entity_manager.entity_max; i++)
	{
		if (!_entity_manager.entity_list[i]._inuse) continue; // skip any inactive entities
		entity_think(&_entity_manager.entity_list[i]);
	}
}

void entity_update(Entity* self)
{
	if (!self) return;
	// any boilerplate update stuff here
	apply_gravity(self, 0.016f);

	if (_active_world)
	{
		self->position.x += self->velocity.x;
		resolveTileCollisionX(self, _active_world);

		self->position.y += self->velocity.y;
		resolveTileCollisionY(self, _active_world);
	}

	if (self->update)self->update(self);
}

void entity_system_update()
{
	int i;
	for (i = 0; i < _entity_manager.entity_max; i++)
	{
		if (!_entity_manager.entity_list[i]._inuse) continue; // skip any inactive entities
		entity_update(&_entity_manager.entity_list[i]);
	}
}

void entity_draw(Entity* self)
{
	GFC_Vector2D offset, position;

	if (!self) return;
	offset = camera_get_offset();
	gfc_vector2d_add(position, self->position, offset);

	if (self->sprite)
	{
		gf2d_sprite_draw(
			self->sprite,
			self->position,
			&self->scale,
			NULL,
			NULL,
			NULL,
			NULL,
			(Uint32)self->frame);
	}
}

void entity_system_draw()
{
	int i;
	for (i = 0; i < _entity_manager.entity_max; i++)
	{
		if (!_entity_manager.entity_list[i]._inuse) continue; // skip any inactive entities
		entity_draw(&_entity_manager.entity_list[i]);
	}
}

void apply_gravity(Entity* self, float deltaTime)
{
	const float gravity = 9.81f; // constant gravity value

	if (!self->onGround)
		self->velocity.y += gravity * deltaTime;
}

void entity_system_set_world(World* world)
{
	_active_world = world;
}