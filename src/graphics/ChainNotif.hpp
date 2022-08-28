#pragma once

#include "entities/Entity.hpp"

// Text notifications of score-chaining
class ChainNotif : public Entity
{
public:
	ChainNotif();
	~ChainNotif();
	void activate(int x, int y, int value);
	void handle();
private:
	Fixed backupX;
	// Number to display
	int value;
	bool maxChain;
	// Stay visible for a certain amount of frames
	int untilDeath;
	int counter;
};
