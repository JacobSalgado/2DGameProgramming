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
    #include "audio.h"
    #include "camera.h"
    #include "gamepad.h" 
    #include "entity.h"
    #include "ring.h"
    #include "spring.h"
    #include "speedpad.h"
    #include "collision.h"
    #include "world.h"
    #include "save.h"
}
#endif

static char _pending_transition[256] = {0};

extern "C" void request_transition(const char* next_level)
{
    snprintf(_pending_transition, 256, "%s", next_level);
}

World* level_transition(World* current_level, Player* sonic, const char* next_level_path, int& ringCount)
{
    if (!next_level_path)
    {
        slog("level_transition: null level path");
        return current_level;
    }

    // saves rings
    SaveData save = save_data_default();
    save.rings = ringCount;
    snprintf(save.current_level_path, 256, next_level_path);
    save_data_write(&save);

    // clear entity and ring pools before freeing the world
    entity_clear_all(sonic->entity);
    ring_system_clear();
    free_world(current_level);

    // load new world
    World* next = world_load(next_level_path);
    if (!next)
    {
        slog("level transition: failed to load %s", next_level_path);
        return NULL;
    }

    // respawn entities
    entity_system_set_world(next);
    world_spawn_entities(next, sonic->entity);
    world_setup_camera(next);

    // restore rings
    ringCount = save.rings;
    //gfc_line_sprintf(ringText, "Rings: %d", ringCount); //updates display

    slog("level_transition: loaded %s, rings=%d", next_level_path, ringCount);
    return next;
}

int main(int argc, char * argv[])
{
    /*variable declarations*/
    int done = 0;
    const Uint8 * keys;
    const char* windowName = "Sonic";

    SDL_GameController* controller = NULL;

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
    audio_init();
    gf2d_sprite_init(1024);
    font_init();
    entity_system_initialize(1024);
    ring_system_init(35);
    //SDL_ShowCursor(SDL_DISABLE);
    camera_set_size(gfc_vector2d(1200, 720));
    
    /*demo setup*/
    Player::destroy_instance();
    Player* sonic =  Player::create_instance(800, 1300);

    int song = audio_load_music("audio/Emerald_Hill_Zone.mp3");
    audio_play_music(song);

    level = world_load("maps/testworld.json");
    entity_system_set_world(level);
    world_spawn_entities(level, sonic->entity);
    world_setup_camera(level);

    // controller setup
    gamepad_init(&controller);

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
        if (_pending_transition[0] != '\0')
        {
            level = level_transition(level, sonic, _pending_transition, ringCount);
            gfc_line_sprintf(ringText, "Rings: %d", ringCount);
            _pending_transition[0] = '\0';
        }

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
    gamepad_close(&controller);
    free_world(level);
    audio_close();
    slog("---==== END ====---");
    return 0;
}
/*eol@eof*/
