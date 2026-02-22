#include "simple_logger.h"

#include "level.h"

World* new_world(Uint32 width, Uint32 height)
{
	World* world;

	if ((!width) || (!height))
	{
		slog("cannot create world with 0 height and width");
		return NULL;
	}

	world = gfc_allocate_array(sizeof(World), 1);
	if (!world)
	{
		slog("failed to allocate a new world");
		return NULL;
	}

	world->tileMap = gfc_allocate_array(sizeof(Uint8), height * width);
	world->tileHeight = height;
	world->tileWidth = width;
	return world;
}

void free_world(World* world)
{
	if (!world) return;

	// free the pointers
	gf2d_sprite_free(world->background);
	gf2d_sprite_free(world->tileSet);
	free(world->tileMap);
	free(world);
}

void draw_world(World* world)
{
	int i, j;
	int frame;
	int index;

	if (!world) return;

	gf2d_sprite_draw_image(world->background, gfc_vector2d(0, 0));
	if (!world->tileSet) return; // if there are no tiles, can't draw anything
	for (j = 0; j < world->tileHeight; j++)
	{
		for (i = 0; i < world->tileWidth; i++)
		{
			index = i + (j*world->tileWidth);
			if (world->tileMap[index] == 0) continue;
			frame = world->tileMap[index];
			gf2d_sprite_draw(
				world->tileSet,
				gfc_vector2d(0,0),
				NULL,
				NULL,
				NULL,
				NULL,
				NULL,
				frame);
		}
	}
}