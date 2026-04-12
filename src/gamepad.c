#include "simple_Logger.h"

#include "gamepad.h"

int xDirection = 0;
int yDirection = 0;

void gamepad_init(SDL_GameController** controller)
{
	SDL_GameControllerAddMappingsFromFile("gamecontrollerdb.txt");

	if (SDL_NumJoysticks() == 0)
	{
		slog("no controllers found");
		return;
	}
	*controller = SDL_GameControllerOpen(0);

	if (!*controller)
	{
		slog("no controller found");
		return;
	}

	slog("%d controller[s] connected", SDL_NumJoysticks());

	if (SDL_GameControllerGetType(controller) == SDL_CONTROLLER_TYPE_PS4)
		slog("PS4 controller connected");
}

void gamepad_close(SDL_GameController** controller)
{
	if (*controller)
	{
		SDL_GameControllerClose(controller);
		*controller = NULL;
	}
}

void gamepad_cleanup()
{
}

void handle_input(SDL_Event* event)
{
	if (event->jaxis.which == 0)
	{
		// x-axis
		if (event->jaxis.axis == 0)
		{
			if (event->jaxis.axis == 0)
			{
				slog("%d xAxis:", event->jaxis.value);
				if (event->jaxis.value > DEAD_ZONE)
					xDirection = 1;
				else if (event->jaxis.value < -DEAD_ZONE)
					xDirection = -1;
				else
					xDirection = 0;
			}
		}

		// y-axis
		if (event->jaxis.axis == 1)
		{
			if (event->jaxis.axis == 1)
			{
				slog("%d yAxis:", event->jaxis.value);
				if (event->jaxis.value > DEAD_ZONE)
					yDirection = 1;
				else if (event->jaxis.value < -DEAD_ZONE)
					yDirection = -1;
				else
					yDirection = 0;
			}
		}
	}
}