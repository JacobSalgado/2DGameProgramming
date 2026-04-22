#include "SDL_mixer.h"

#include "simple_logger.h"

#include "audio.h"



void audio_init()
{
	Mix_Init(MIX_INIT_MP3);
}

void audio_load(const char* filename)
{
		
}

int audio_load_music(const char* filename)
{
	Mix_Music* music = NULL;
	music = Mix_LoadMUS(filename);
	if (music == NULL)
	{
		slog("failed to load music. SDL_Mixer error: %s\n", Mix_GetError());
		return -1;
	}
	//music.pusb
}

int audio_load_sound(const char* filename)
{
}

void audio_play_music(int music)
{
}

void audio_play_sound(int sound)
{
}

void set_volume(int volume)
{
}

void audio_play()
{
}

void audio_close()
{
}

void audio_cleanup()
{
}