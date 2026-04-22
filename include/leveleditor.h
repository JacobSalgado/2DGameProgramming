#ifndef __LEVELEDITOR_H__
#define __LEVELEDITOR_H__

#include "gfc_types.h"
#include "camera.h"

#define EDITOR_SCREEN_W 1200
#define EDITOR_SCREEN_H 720

#define EDITOR_MAX_OBJECTS 2048
#define EDITOR_CAM_SPEED 6.0f
#define EDITOR_SNAP 16

typedef enum {
	ENTITY_RING = 0,
	ENTITY_SPRING,
	ENTITY_SPEEDPAD,
	ENTITY_ENEMY_TURRET,
	ENTITY_ENEMY_FLY,
	ENTITY_COUNT
} EditorEntityType;

typedef struct {
	Uint8 _inuse;
	EditorEntityType type;
	GFC_Vector2D position;
} EditorObject;

typedef struct {
	Uint8 active;
	EditorEntityType currentEntityType;
	GFC_Vector2D cursor;
	float cameraSpeed;
} EditorState;

/**
* @brief initializes the editor
* @param editor the state of the editor
*/
void editor_init(EditorState* editor);

/**
* @brief listens for 1 key toggle, mouse placement, and camera panning
* @param editor the state of the editor
*/
void editor_update(EditorState* editor);

/**
* @brief draws user edits
*/
void editor_draw(EditorState* editor);

/**
* @brief saves all the objects to a JSON file
* @return 1 on success, 0 on fail
*/
int editor_save(EditorState* editor, const char* filename);

/**
* @brief loads in a saved edit from a JSON file; clears any previous edits
* @return 1 on success, 0 on fail
*/
int editor_load(EditorState*, const char* filename);

/**
* @brief clears the board/objects
*/
void editor_clear(EditorState* editor);

/**
* @brief looks for a matching entity type name
*/
const char* editor_type_name(EditorEntityType type);


#endif