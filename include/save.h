#ifndef __SAVE_H__
#define __SAVE_H__

typedef struct {
	int rings;
	int current_level;
	char current_level_path[256];
	char music_path[256];
} SaveData;

/**
* @brief writes saved data to a file
*/
void save_data_write(SaveData* data);

/**
* @brief loads from file, returns default if missing
*/
SaveData save_data_read();

/**
* @brief loads standard brand new game
*/
SaveData save_data_default();

#endif