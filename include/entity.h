#ifndef __ENTITY_H__
#define __ENTITY_H__

#include <SDL.h>
#include "gfc_types.h"
#include "gf2d_sprite.h"

/**
*  @purpose whats the point of this file
*/

typedef struct Entity_S
{
	Uint8			_inuse; /**<this is the flag for keeping track of memory usage*/
	Sprite			*sprite; /*<the entity's graphic if it has one*/
	GFC_Vector2D	position; /**<the entity's position in the world*/
	float			rotation;
	GFC_Vector2D	scale;
	float			frame; /*<the current frame of animation for the sprite*/
	void (*think)(struct Entity_S* self); /*<function to call to make decisions*/
	void (*update)(struct Entity_S* self); /*function to call to execute those decisions*/
	void (*free) (struct Entity_S* self); /*clean upp any custom allocated data*/
	void *data;								/* for ad hoc addition data for the entity */
}Entity;
 


/**
* @brief initialize the entity management system and queues up cleanup on exit
* @param max the maximum number of entities that can exist at the same time
*/
void entity_manager_init(Uint32 max);

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

#endif#pragma once
