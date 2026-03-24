#include <SDL.h>

#include "player.h"
#include "enemy.h"
#include "fly_enemy.h"
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
    //Sprite *sprite;
    const char* windowName = "Sonic";

    World* level;
    
    //int mx,my;
    float mf = 0;
    int lives = 3;
    int ringCount = 0;
    int enemyCount = 0;
    int enemyMax = 15;
    const char* uiText = "Sonic Health:";
    GFC_TextLine ringText;
    gfc_line_sprintf(ringText, "Rings: %d", ringCount);
    //const char* ringText = "Rings: ";
    Sprite *mouse;
    //GFC_Color mouseGFC_Color = gfc_color8(100,180,250,200);
    //Entity *player;
    //Entity* enemy;
    //Enemy* e;
    Entity* ring;
    Entity* spring;
    Entity* speedpad;

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
    //SDL_ShowCursor(SDL_DISABLE);
    camera_set_size(gfc_vector2d(1200, 720));
    
    /*demo setup*/
    //sprite = gf2d_sprite_load_image("images/backgrounds/puck_guts.jpg");
    //mouse = gf2d_sprite_load_all("images/pointer.png",32,32,16,0);
    //player = player_new(); /* initialize the player */
    Player::destroy_instance();
    Player* sonic =  Player::create_instance(800, 2000);

    level = world_load("maps/testworld.json");
    entity_system_set_world(level);
    //enemy = enemy_new(1000, 200); /* initialize the enemy */
    //enemy = enemy_new(800, 300);
    //enemy = enemy_new(1000, 500);
    //ring  = ring_new(player->position.x, player->position.y); /* initialize rings */
    ring = ring_new(1400, 400);
    spring = spring_new(600, 1600);
    speedpad = speedpad_new(1400, 1750);

    world_setup_camera(level);

    slog("press [escape] to quit");
    /*main game loop*/
    while(!done)
    {
        Uint32 currentTime = SDL_GetTicks();

        SDL_Event event;
        //SDL_PumpEvents();   // update SDL's internal event structures
        keys = SDL_GetKeyboardState(NULL); // get the keyboard state for this frame
        font_cleanup();
        /*update things here*/
        //SDL_GetMouseState(&mx,&my);
        mf+=0.1;
        if (mf >= 16.0)mf = 0;

        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                done = true;
            }
            sonic->handle_input(&event);
        }
        //player_move(player);

        entity_system_think();
        entity_system_update();

        //apply_gravity(player, 0.016f);

        //entity_check_collisions(sonic);
        //entity_surface_collision(level, player);
        if (ring && ring_collect(ring, sonic->entity))
        {
            ringCount++;
            gfc_line_sprintf(ringText, "Rings: %d", ringCount);
            ring = NULL;
        }
        if (spring) spring_activate(spring, sonic->entity);
        if (speedpad) speedpad_activate(speedpad, sonic->entity);

        if ((currentTime - lastSpawnTime >= spawnDelay) && enemyCount <= enemyMax)
        {
            //enemy_new((rand() % 651 + 50), (rand() % 200 + 50));
            Enemy* e = new FlyEnemy((rand() % 1001 + 50), (rand() % 251 + 50));
            enemyCount++;
            //Enemy* g = new GroundEnemy(420, 300);
            lastSpawnTime = currentTime;
        }
        
        gf2d_graphics_clear_screen();// clears drawing buffers
        // all drawing should happen betweem clear_screen and next_frame
            //backgrounds drawn first
            //gf2d_sprite_draw_image(sprite,gfc_vector2d(0,0));
            draw_world(level);

            entity_system_draw();
            
            font_draw_text(uiText, FS_small, GFC_COLOR_DARKBLUE, gfc_vector2d(10, 10));
            font_draw_text(ringText, FS_small, GFC_COLOR_DARKYELLOW, gfc_vector2d(10, 100));
            
            //UI elements last
            /*gf2d_sprite_draw(
                mouse,
                gfc_vector2d(mx,my),
                NULL,
                NULL,
                NULL,
                NULL,
                &mouseGFC_Color,
                (int)mf);*/

        gf2d_graphics_next_frame();// render current draw frame and skip to the next frame
        
        if (keys[SDL_SCANCODE_ESCAPE])done = 1; // exit condition
        //slog("Rendering at %f FPS",gf2d_graphics_get_frames_per_second());
    }
    //entity_free(ring);
    //entity_free(enemy);
    //entity_clear_all(sonic);
    //entity_free(player);
    sonic->~Player();
    free_world(level);
    slog("---==== END ====---");
    return 0;
}
/*eol@eof*/
