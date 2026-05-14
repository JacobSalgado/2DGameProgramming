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

typedef enum
{
    STATE_MENU,
    STATE_PAUSED,
    STATE_GAME,
    STATE_QUIT
} GameState;

typedef struct
{
    int selected;
    bool has_save;
    bool is_paused;
    Sprite* background;
} MenuState;

MenuState menu_init()
{
    MenuState menu = { 0 };
    menu.selected = 0;
    menu.background = gf2d_sprite_load_image("images/backgrounds/menu.jpg");

    // check if save exists
    SaveData save = save_data_read();
    menu.has_save = (save.rings > 0 || strlen(save.current_level_path) > 0);
    return menu;
}

void menu_free(MenuState* menu)
{
    if (!menu) return;
    gf2d_sprite_free(menu->background);
}

void menu_draw(MenuState* menu)
{
    if (!menu) return;

    // draw background
    if (menu->background)
        gf2d_sprite_draw_image(menu->background, gfc_vector2d(0, 0));

    // draw title
    font_draw_text("Sonic the Hedgehog", FS_large, GFC_COLOR_YELLOW, gfc_vector2d(500,150));

    //int y_offset = 300;

    if (menu->is_paused)
    {
        GFC_Color c = (menu->selected == 0) ? GFC_COLOR_YELLOW : GFC_COLOR_WHITE;
        //font_draw_text("Resume", FS_medium, c, gfc_vector2d(500, y_offset));
        //y_offset += 70;
        font_draw_text("Resume", FS_medium, menu->selected == 0 ? GFC_COLOR_YELLOW : GFC_COLOR_WHITE, gfc_vector2d(500, 280));
        font_draw_text("Start New", FS_medium, menu->selected == 1 ? GFC_COLOR_YELLOW : GFC_COLOR_WHITE, gfc_vector2d(500, 350));
        font_draw_text("Continue", FS_medium, menu->selected == 2 ? GFC_COLOR_YELLOW : (menu->has_save ? GFC_COLOR_WHITE : GFC_COLOR_DARKGREY), gfc_vector2d(500, 420));
        font_draw_text("Quit", FS_medium, menu->selected == 3 ? GFC_COLOR_YELLOW : GFC_COLOR_WHITE, gfc_vector2d(500, 490));
    }
    else
    {
        font_draw_text("Start Game", FS_medium, menu->selected == 0 ? GFC_COLOR_YELLOW : GFC_COLOR_WHITE, gfc_vector2d(500, 300));
        font_draw_text("Continue", FS_medium, menu->selected == 2 ? GFC_COLOR_YELLOW : (menu->has_save ? GFC_COLOR_WHITE : GFC_COLOR_DARKGREY), gfc_vector2d(500, 370));
        font_draw_text("Quit", FS_medium, menu->selected == 3 ? GFC_COLOR_YELLOW : GFC_COLOR_WHITE, gfc_vector2d(500, 440));
    }

    int start_idx = menu->is_paused ? 1 : 0;
    int continue_idx = menu->is_paused ? 2 : 1;
    int quit_idx = menu->is_paused ? 3 : 2;

    // draw menu options
    //GFC_Color start_color = (menu->selected == 0) ? GFC_COLOR_YELLOW : GFC_COLOR_WHITE;
    //GFC_Color continue_color = (menu->selected == 1) ? GFC_COLOR_YELLOW : (menu->has_save ? GFC_COLOR_WHITE : GFC_COLOR_DARKGREY);
    //GFC_Color quit_color = (menu->selected == 2) ? GFC_COLOR_YELLOW : GFC_COLOR_WHITE;

    
}

GameState menu_handle_input(SDL_Event* event, MenuState* menu)
{
    if (!event || !menu) return STATE_MENU;

    int max_option = menu->is_paused ? 3 : 2;

    if (event->type == SDL_KEYDOWN)
    {
        switch (event->key.keysym.scancode)
        {
        case SDL_SCANCODE_UP:
            menu->selected--;
            if (menu->selected < 0) menu->selected = max_option;
            // skip continue if no save (index 2 paused, index 1 normal)
            if (menu->is_paused && menu->selected == 2 && !menu->has_save)
                menu->selected--;
            if (!menu->is_paused && menu->selected == 1 && !menu->has_save)
                menu->selected--;
            break;

        case SDL_SCANCODE_DOWN:
            menu->selected++;
            if (menu->selected > max_option) menu->selected = 0;
            // skip continue if no save
            if (menu->is_paused && menu->selected == 2 && !menu->has_save)
                menu->selected++;
            if (!menu->is_paused && menu->selected == 1 && !menu->has_save)
                menu->selected++;
            break;

        case SDL_SCANCODE_RETURN:
        case SDL_SCANCODE_SPACE:
            if (menu->is_paused)
            {
                if (menu->selected == 0) return STATE_GAME;  // Resume
                if (menu->selected == 1) return STATE_GAME;  // Start New
                if (menu->selected == 2 && menu->has_save) return STATE_GAME; // Continue
                if (menu->selected == 3) return STATE_QUIT;  // Quit
            }
            else
            {
                if (menu->selected == 0) return STATE_GAME;  // Start
                if (menu->selected == 1 && menu->has_save) return STATE_GAME; // Continue
                if (menu->selected == 2) return STATE_QUIT;  // Quit
            }
            break;

        case SDL_SCANCODE_ESCAPE:
            if (menu->is_paused) return STATE_GAME; // escape resumes
            break;

        default: break;
        }
    }
    return menu->is_paused ? STATE_PAUSED : STATE_MENU;
}

World* level_transition(World* current_level, Player* sonic, const char* next_level_path, int& ringCount)
{
    if (!next_level_path)
    {
        slog("level_transition: null level path");
        return current_level;
    }

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

    // saves rings
    SaveData save = save_data_default();
    save.rings = ringCount;
    snprintf(save.current_level_path, 256, next_level_path);
    snprintf(save.music_path, 256, "%s", next->music_path);
    save_data_write(&save);

    // respawn entities
    entity_system_set_world(next);
    world_spawn_entities(next, sonic->entity);
    sonic->entity->position = gfc_vector2d(next->player_start_x, next->player_start_y);
    sonic->entity->velocity = gfc_vector2d(0, 0);
    world_setup_camera(next);

    // change music
    audio_stop_music();
    if (strlen(next->music_path) > 0)
    {
        int song = audio_load_music(next->music_path);
        audio_play_music(song);
    }

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

    World* level = NULL;

    GameState state = STATE_MENU;
    MenuState menu;
    
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

    menu = menu_init();
    // controller setup
    gamepad_init(&controller);
    slog("press [escape] to quit");
    
    /*demo setup*/
    //Player::destroy_instance();
    //Player* sonic =  Player::create_instance(800, 1300);
    Player* sonic = NULL;

    //int song = audio_load_music("audio/Emerald_Hill_Zone.mp3");
    //audio_play_music(song);

    //level = world_load("maps/testworld.json");
    //entity_system_set_world(level);
    //world_spawn_entities(level, sonic->entity);
    //world_setup_camera(level);


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
                break;
            }

            if (state == STATE_MENU)
            {
                state = menu_handle_input(&event, &menu);

                // transition to game, set everything up
                if (state == STATE_GAME)
                {
                    Player::destroy_instance();
                    sonic = Player::create_instance(800, 1300);

                    SaveData save = save_data_default();
                    bool is_continue = (menu.selected == 1 && menu.has_save);
                    if (is_continue)
                        save = save_data_read();

                    ringCount = save.rings;
                    gfc_line_sprintf(ringText, "Rings: %d", ringCount);

                    const char* start_level = is_continue ? save.current_level_path : "maps/testworld.json";
                    level = world_load(start_level);
                    entity_system_set_world(level);
                    world_spawn_entities(level, sonic->entity);
                    world_setup_camera(level);

                    int song = audio_load_music("audio/Emerald_Hill_zone.mp3");
                    audio_play_music(song);

                    menu_free(&menu);
                }
            }
            else if (state == STATE_GAME)
            {
                sonic->handle_input(&event);
            }
            else if (state == STATE_PAUSED)
            {
                GameState new_state = menu_handle_input(&event, &menu);
                if (new_state == STATE_GAME)
                {
                    menu.is_paused = false;
                    state = STATE_GAME;
                }
                else if (new_state == STATE_QUIT)
                {
                    done = true;
                }
            }
        }

        if (state == STATE_QUIT) done = 1;

        gf2d_graphics_clear_screen();

        if (state == STATE_MENU)
        {
            menu_draw(&menu);
        }
        else if (state == STATE_GAME)
        {
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

            if (_pending_transition[0] != '\0')
            {
                level = level_transition(level, sonic, _pending_transition, ringCount);
                gfc_line_sprintf(ringText, "Rings: %d", ringCount);
                _pending_transition[0] = '\0';
            }

            draw_world(level);
            entity_system_draw();
            ring_system_draw();

            font_draw_text(uiText, FS_small, GFC_COLOR_LIGHTCYAN, gfc_vector2d(10, 10));
            font_draw_text(ringText, FS_small, GFC_COLOR_LIGHTGREEN, gfc_vector2d(1000, 10));

            if (keys[SDL_SCANCODE_ESCAPE])
            {
                menu.is_paused = true;
                menu.selected = 0;
                state = STATE_PAUSED;
            }
        }
        else if (state == STATE_PAUSED)
        {
            draw_world(level);
            entity_system_draw();
            ring_system_draw();
            menu_draw(&menu);
        }

        gf2d_graphics_next_frame();

        /*
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
        //slog("Rendering at %f FPS",gf2d_graphics_get_frames_per_second());*/
    }
    //sonic->~Player();
    if (level) free_world(level);
    gamepad_close(&controller);
    audio_close();
    slog("---==== END ====---");
    return 0;
}
/*eol@eof*/
