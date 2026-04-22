#ifndef __SONIC_H__
#define __SONIC_H__

#include "player.h"

class Sonic : public Player
{
public:

	/**
	* @brief constructor
	*/
	Sonic(int x, int y);

	/**
	* @brief deconstructor
	*/
	~Sonic();

	void update() override;

	void think() override;

	static Sonic* create_instance(int x, int y);

private:
};

#endif