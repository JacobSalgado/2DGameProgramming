#ifndef __GOALPOST_H__
#define __GOALPOST_H__

#include "entity.h"

typedef struct
{
	char next_level[256];
	int triggered;
} GoalPostData;

Entity* goalpost_new(float x, float y, const char* next_level);

#endif
