#ifndef __ENTITY_H__
#define __ENTITY_H__

#include <SDL.h>
#include "gfc_types.h"
#include "gf2d_sprite.h"

//#include "world.h"

/* Set up for C function definitions, even when using C++ */
#ifdef __cplusplus
extern "C" 
{
	
}
#endif

struct World_S;
typedef struct World_S World;

/**
*  @purpose whats the point of this file
*/

typedef enum
{
	ENTITY_TYPE_PLAYER,
	ENTITY_TYPE_ENEMY,
	ENTITY_TYPE_RING,
	ENTITY_TYPE_SPRING,
	ENTITY_TYPE_SPEEDPAD,
	ENTITY_TYPE_PROJECTILE,
	ENTITY_TYPE_GOALPOST
}EntityType;

typedef struct Entity_S
{
	EntityType		type;
	Uint8			_inuse; /**<this is the flag for keeping track of memory usage*/
	Sprite			*sprite; /*<the entity's graphic if it has one*/
	GFC_Vector2D	position; /**<the entity's position in the world*/
	float			rotation;
	GFC_Vector2D	scale;
	GFC_Vector2D	velocity;	/**<how much an entity's position changes per update>*/
	float			frame; /*<the current frame of animation for the sprite*/
	float			lifeTime; /*<how long the entity will live>*/
	float			mass; /*<mass of the entity>*/
	Uint8			width; /*width of the entity*/
	Uint8			height; /*height of the entity*/
	Uint8			health; /* amount of health the entity has */
	Uint8			onGround; /* <if the entity is on the ground >*/
	Uint8			gravityOn; /* < not all entities will be bound by gravity >*/
	float			boost_timer;
	float			boost_velocity;

	void (*think)(struct Entity_S* self); /*<function to call to make decisions*/
	void (*update)(struct Entity_S* self); /*function to call to execute those decisions*/
	void (*free) (struct Entity_S* self); /*clean upp any custom allocated data*/
	void *data;								/* for ad hoc addition data for the entity */
}Entity;
 


/**
* @brief initialize the entity management system and queues up cleanup on exit
* @param max the maximum number of entities that can exist at the same time
*/
void entity_system_initialize(Uint32 max);

/**
* @brief clean up all active entities
* @param ignore do not clean up this entity
*/
void entity_clear_all(Entity *ignore);

/**
* @brief get a pointer to a free entity
* @return NULL if out of entities, a pointer to a blank entity otherwise
*/
Entity* entity_new();

/**
* @brief clean up an entity, and free its spot for future use
* @param self the entity to free
* @ note do not use the memory address again after call this
*/
void entity_free(Entity* self);

void entity_destroy(Entity* self);

void entity_manager_remove(Entity* self);

/**
* @brief run the think functions for all active entities
*/
void entity_system_think();

/**
* @brief run the update functions for all active entities
*/
void entity_system_update();

/**
* @brief draw all active entities
*/
void entity_system_draw();

void entity_check_collisions(Entity* player);

//void entity_surface_collision(World* world, Entity* player);

/**
* @brief applies gravity to the entity
* @@param self the entity to apply gravity to
* Credit to: https://peerdh.com/blogs/programming-insights/implementing-a-simple-physics-engine-in-c-with-sdl
*/
void apply_gravity(Entity* self, float deltaTime);

void entity_system_set_world(World* world);

/**
* @brief checks if an enemy type is colliding while the player is jumping
* @param player the player colliding with
*/
int enemy_collide_check(Entity* player);

Uint32 entity_system_get_max();

Entity* entity_system_get(int index);

void entity_set_player(Entity* player);

Entity* entity_get_player();


#endif
