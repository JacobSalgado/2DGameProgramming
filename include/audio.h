#ifndef __AUDIO_H__
#define __AUDIO_H__

#include "gfc_audio.h"

/**
* @brief initializes audio system
*/
void audio_init();

void audio_close();

/**
* @brief cleans up any internal cache
*/
void audio_cleanup();

#endif
