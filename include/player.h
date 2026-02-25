#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "entity.h"

/**
* @brief spawn a player
* @return NULL on error, or a pointer to the player otherwise
*/
Entity *player_new();

/**
* @brief what the player should do
* @param self the player
*/
void player_think(Entity* self);

/**
* @brief the player to update
* @param self the player
*/
void player_update(Entity* self);

/**
* @brief free the player
* @param self the player to free
*/
void player_free(Entity* self);

/**
* @brief handles any play input commands
* @param self the player to control
* @param event the event pointer address
*/
void player_input(Entity* self, SDL_Event* event);

/**
* @brief makes the player move
* @param self the player to move
*/
void player_move(Entity* self);

#endif