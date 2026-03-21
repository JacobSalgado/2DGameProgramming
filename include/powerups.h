#ifndef __POWERUPS_H__
#define __POWERUPS_H__

#include "simple_json.h"

/**
* @brief initialize and load powerup definitions
* @param filename the json file containing item data
*/
void powerups_initialize(const char* filename);

/**
* @brief get a powerup definition by its name
* @param name the search criteria
* @return NULL if not found, otherwise the definition information of the item
* @note DO NOT FREE THAT DATA 
*/
SJson* powerups_get_def_by_name(const char* name);

#endif
