#include <SDL.h>

#include "player.h"
#include "enemy.h"
#include "fly_enemy.h"
#include "turret_enemy.h"
#include "ground_enemy.h"

/* Set up for C function definitions, even when using C++ */
#ifdef __cplusplus
extern "C"
{
    #include "simple_logger.h"

    #include "gf2d_graphics.h"
    #include "gf2d_sprite.h"

    #include "font.h"
    #include "camera.h"
    #include "entity.h"
    #include "ring.h"
    #include "spring.h"
    #include "speedpad.h"
    #include "collision.h"
    #include "world.h"
}
#endif



int main(int argc, char * argv[])
{
    /*variable declarations*/
    int done = 0;
    const Uint8 * keys;
    const char* windowName = "Sonic";

    World* level;
    
    int lives = 3;
    int ringCount = 0;
    int enemyCount = 0;
    int enemyMax = 15;
    const char* uiText = "Sonic Health:";
    GFC_TextLine ringText;
    gfc_line_sprintf(ringText, "Rings: %d", ringCount);

    Uint32 lastSpawnTime = SDL_GetTicks();
    Uint32 spawnDelay = 3000; // 3 seconds in milliseconds
    
    /*program initializtion*/
    init_logger("gf2d.log",0);
    slog("---==== BEGIN ====---");
    gf2d_graphics_initialize(
        "Sonic",
        1200,
        720,
        1200,
        720,
        gfc_vector4d(0,0,0,255),
        0);
    gf2d_graphics_set_frame_delay(16);
    gf2d_sprite_init(1024);
    font_init();
    entity_system_initialize(1024);
    ring_system_init(35);
    //SDL_ShowCursor(SDL_DISABLE);
    camera_set_size(gfc_vector2d(1200, 720));
    
    /*demo setup*/
    Player::destroy_instance();
    Player* sonic =  Player::create_instance(800, 1500);

    level = world_load("maps/testworld.json");
    entity_system_set_world(level);
    world_spawn_entities(level, sonic->entity);
    world_setup_camera(level);

    slog("press [escape] to quit");
    /*main game loop*/
    while(!done)
    {
        Uint32 currentTime = SDL_GetTicks();

        SDL_Event event;
        keys = SDL_GetKeyboardState(NULL); // get the keyboard state for this frame
        font_cleanup();
        /*update things here*/

        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                done = true;
            }
            sonic->handle_input(&event);
        }

        entity_system_think();
        entity_system_update();
        world_update_moving_platforms(level);

        for (int i = 0; i < ring_system_get_max(); i++)
        {
            Entity* ring = ring_system_get(i);
            if (!ring || !ring->_inuse) continue;
            if (ring_collect(ring, sonic->entity))
            {
                ringCount++;
                gfc_line_sprintf(ringText, "Rings: %d", ringCount);
            }
        }

        for (int i = 0; i < entity_system_get_max(); i++)
        {
            Entity* ent = entity_system_get(i);
            if (!ent || !ent->_inuse) continue;
            if (ent->type == ENTITY_TYPE_SPRING)
                spring_activate(ent, sonic->entity);
            else if (ent->type == ENTITY_TYPE_SPEEDPAD)
                speedpad_activate(ent, sonic->entity);
        }
        enemy_collide_check(sonic->entity);
        
        gf2d_graphics_clear_screen();// clears drawing buffers
        // all drawing should happen betweem clear_screen and next_frame
            //backgrounds drawn first
            draw_world(level);

            entity_system_draw();
            ring_system_draw();
            
            font_draw_text(uiText, FS_small, GFC_COLOR_LIGHTCYAN, gfc_vector2d(10, 10));
            font_draw_text(ringText, FS_small, GFC_COLOR_LIGHTGREEN, gfc_vector2d(1000, 10));
            

        gf2d_graphics_next_frame();// render current draw frame and skip to the next frame
        
        if (keys[SDL_SCANCODE_ESCAPE])done = 1; // exit condition
        //slog("Rendering at %f FPS",gf2d_graphics_get_frames_per_second());
    }
    sonic->~Player();
    free_world(level);
    slog("---==== END ====---");
    return 0;
}
/*eol@eof*/
