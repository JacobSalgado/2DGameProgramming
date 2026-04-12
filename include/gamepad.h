#ifndef __GAMEPAD_H__
#define __GAMEPAD_H__

#include "SDL_gamecontroller.h"
#include "SDL_events.h"

// Deadzone values - this is so the controller is not too sensitive
const int DEAD_ZONE = 8000;
extern int xDirection;
extern int yDirection;

void gamepad_init(SDL_GameController** controller);

void gamepad_close(SDL_GameController** controller);

void gamepad_cleanup();

void handle_input(SDL_Event* event);

void gamepad_update(SDL_Event* event);

// getters
int get_gamepad_x_direction();

int get_gamepad_y_direction();

int gamepad_button_pressed(SDL_GameControllerButton button);

#endif