#pragma once

#include "types.h"
#include "entities/Entity.hpp"

// Both enemies and player can fire them
class Bullet : public Entity
{
public:
	Bullet();
	~Bullet();
	Rect* makeRect();
	void activate(Fixed x, Fixed y, Fixed a, Fixed r, int imageID, bool polarity, bool hurtsPlayer, int camRelation);
	bool getPolarity();
	bool hurtsPlayer();
	bool handle();
	void draw();
	Fixed getx();
	Fixed gety();
	// speed
	Fixed dx, dy;
	unsigned short* img;
protected:
	bool polarity;
	bool hurtPlayer;
};
