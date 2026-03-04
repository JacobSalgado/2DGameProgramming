#ifndef __LEVEL_H__
#define __LEVEL_H__

#include "gf2d_sprite.h"

typedef struct
{
	Sprite* background;		/**<background image for the world>*/
	Sprite* tileLayer;		/**<prerendered sprite layer>*/
	Sprite* tileSet;		/**<sprite containing the tiles for the world>*/
	Uint8* tileMap;			/**<the tiles that make up the world>*/
	Uint32 tileHeight;		/**<how many tiles tall the map is>*/
	Uint32 tileWidth;		/**<how many tiles wide the map is>*/
}
World;

void world_tile_layer_build(World* world);

/**
* @brief load a world from a config file 
* @param filename the name of the world file to load
* @return NULL on error, or a usable world otherwise 
*/
World* world_load(const char* filename);

/**
* @brief set the camera bounds to the world size
*/
void world_setup_camera(World* world);

/**
* @brief create a brand new world
* @param width the width of the tiles 
* @param height the height of the tiles
* @return NULL for errors, or an empty world
*/
World* new_world(Uint32 width, Uint32 height);

/**
* @brief free a previously assigned world
* @param world the world to free
*/
void free_world(World* world);

/**
* @brief draw the world
* @return world the world to draw
*/
void draw_world(World* world);

#endif