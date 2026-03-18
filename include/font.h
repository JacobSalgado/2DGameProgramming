#ifndef __FONT_H__
#define __FONT_H__

#include "gfc_text.h"
#include "gfc_color.h"

typedef enum
{
	FS_small,
	FS_medium,
	FS_large,
	FS_MAX
}FontStyles;

/**
* @brief initialize the font system
* auto closes on exit
*/
void font_init();

void font_close();

/**
* @brief call this to clean up any internal cache
*/
void font_cleanup();

/**
* @brief render text to the screen
* @param text what to render
* @param style what style to render with
* @param color color of the text
* @param postion where on the screen to draw it to
*/
void font_draw_text(const char* text, FontStyles style, GFC_Color color, GFC_Vector2D position);

#endif
