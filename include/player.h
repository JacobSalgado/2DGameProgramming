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

	/**
	* @brief returns the player's current bounding rectangle for collision
	* @return SDL_Rect representing the player's position and dimensions
	*/
	virtual SDL_Rect rect();

private:
	/**
	* @brief private constructor, prevents direct instantiation
	* @note use get_instance() instead
	* @param x the starting x position
	* @param y the starting y position
	*/
	Player(int x, int y);	

	static Player* _instance; /// < the single instance of the player

	bool moving = false; // for player animation
	bool running = false; // running animation
};

#endif