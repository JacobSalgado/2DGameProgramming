#include "simple_logger.h"

#include "gf2d_graphics.h"

#include "camera.h"
#include "level.h"

void world_tile_layer_build(World* world)
{
	int i, j;

	GFC_Vector2D position;
	Uint32 frame;

	Uint32 index;

	if (!world) return;

	if (!world->tileSet) return;

	if (world->tileLayer)
	{
		gf2d_sprite_free(world->tileLayer);
	}
	world->tileLayer = gf2d_sprite_new();

	world->tileLayer->surface = gf2d_graphics_create_surface(
		world->tileWidth * world->tileSet->frame_w, 
		world->tileHeight * world->tileSet -> frame_h);

	world->tileLayer->frame_w = world->tileWidth * world->tileSet->frame_w;
	world->tileLayer->frame_h = world->tileHeight * world->tileSet->frame_h;

	if (!world->tileLayer->surface)
	{
		slog("failed to create tilelayer surface");
		return;
	}

	for (j = 0; j < world->tileHeight; j++)
	{
		for (i = 0; i < world->tileWidth; i++)
		{
			index = i + (j * world->tileWidth);
			if (world->tileMap[index] == 0) continue;

			position.x = i * world->tileSet->frame_w;
			position.y = j * world->tileSet->frame_h;

			frame = world->tileMap[index] - 1;

			gf2d_sprite_draw_to_surface(
				world->tileSet,
				position,
				NULL,
				NULL,
				frame,
				world->tileLayer->surface);
		}
	}
	world->tileLayer->texture = SDL_CreateTextureFromSurface(gf2d_graphics_get_renderer(), world->tileLayer->surface);
	if (!world->tileLayer->texture)
	{
		slog("failed to convert world tile layer to texture");
		return;
	}
}

World* world_test_new()
{
	int i;
	int width = 75, height = 45;

	World* world;

	world = new_world(width, height);

	if (!world) return NULL;

	world->background = gf2d_sprite_load_image("images/backgrounds/greenhill.jpg");
	world->tileSet = gf2d_sprite_load_all(
		"images/backgrounds/tileset.png",
		16,
		16,
		1,
		1);

	for (i = 0; i < width; i++)
	{
		world->tileMap[i] = 1;
		world->tileMap[i + ((height - 1) * width)] = 1;
	}
	for (i = 0; i < height; i++)
	{
		world->tileMap[i*width] = 1;
		world->tileMap[i * width + (width - 1)] = 1;
	}
	world_tile_layer_build(world);

	return world;
}

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
	gf2d_sprite_free(world->tileLayer);
	free(world->tileMap);
	free(world);
}

void draw_world(World* world)
{
	//int i, j;
	//int frame;
	//int index;

	GFC_Vector2D position;
	GFC_Vector2D offset;

	if (!world) return;

	offset = camera_get_offset();

	gf2d_sprite_draw_image(world->background, gfc_vector2d(0, 0));
	gf2d_sprite_draw_image(world->tileLayer, offset);

	/*for (j = 0; j < world->tileHeight; j++)
	{
		for (i = 0; i < world->tileWidth; i++)
		{
			index = i + (j*world->tileWidth);
			if (world->tileMap[index] == 0) continue;

			position.x = i * world->tileSet->frame_w;
			position.y = j * world->tileSet->frame_h;
			frame = world->tileMap[index];

			gf2d_sprite_draw(
				world->tileSet,
				position,
				NULL,
				NULL,
				NULL,
				NULL,
				NULL,
				frame);
		}
	}*/
}

void world_setup_camera(World* world)
{
	if (!world) return;
	if ((!world->tileLayer) || (!world->tileLayer->surface))
	{
		slog("no tile layer set for world");
		return;
	}

	camera_set_bounds(gfc_rect(0,0, world->tileLayer->surface->w, world->tileLayer->surface->h));
	camera_apply_bounds();
	camera_enable_binding(1);
}