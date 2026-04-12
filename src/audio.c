#include "SDL_mixer.h"

#include "simple_logger.h"

#include "audio.h"

void audio_init()
{
	Mix_Init(MIX_INIT_MP3);
}

void audio_close()
{
}

void audio_cleanup()
{
}