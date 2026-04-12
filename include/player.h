#ifndef __PLAYER_H__
#define __PLAYER_H__

/* Set up for C function definitions, even when using C++ */
#ifdef __cplusplus
extern "C" 
{
	#include "entity.h"

	int player_is_jumping(Entity* player);
}
#endif

enum PlayerAnimation
{
	ANIM_Idle = 0,
	ANIM_Run,
	ANIM_Jump,
	ANIM_COUNT
};

struct AnimationDef
{
	const char* filename;
	int frame_w;
	int frame_h;
	int frames_per_row;
	int start_frame;
	int end_frame;
};

/**
* @brief represents the player entity in the game
* @note singleton class, use get_instance() to access it
*/
class Player
{
public:
	struct Inputs
	{
		bool left = false;
		bool right = false;
		bool up = false;
		bool jump = false;
	};
	Inputs input;

	Entity* entity; /// < pointer into existing C pool

	Uint8 moving = false;
	Uint8 jumping = false;

	/**
	* @brief deconstructor, cleans up the player entity and frees the singleton instance
	*/
	virtual ~Player();

	/**
	* @brief destroys the singleton instance and frees all associated memory
	*/
	static Player* create_instance(int x, int y);

	/**
	* @brief destroys the singleton instance and frees all associated memory
	*/
	static void destroy_instance();

	/**
	* @brief handles player decision making logic each game tick
	*/
	virtual void think();

	/**
	* @brief updates player position, animation, and state each game tick
	*/
	virtual void update();

	/**
	* @brief processes SDL input events for player control
	* @param event pointer to the SDL_Event to process
	*/
	virtual void handle_input(SDL_Event* event);

private:
	/**
	* @brief private constructor, prevents direct instantiation
	* @note use get_instance() instead
	* @param x the starting x position
	* @param y the starting y position
	*/
	Player(int x, int y);	

	static Player* _instance; /// < the single instance of the player

	Uint8 was_moving = false;
	Uint8 was_jumping = false;
	Uint8 was_on_ground = false;

	Sprite* sprites[ANIM_COUNT];
	PlayerAnimation current_anim;
	PlayerAnimation previous_anim;

	void set_animation(PlayerAnimation anim);
};

#endif