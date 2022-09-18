#pragma once

#include "types.h"
#include "entities/Entity.hpp"
#include "helpers/Constants.hpp"

// Both enemies and player can fire them
class Bullet : public Entity
{
public:
	Bullet();
	Rect* makeRect();
	void activate(Fixed x, Fixed y, Fixed a, Fixed r, LUTs::BaseImageId imageID, bool polarity, bool hurtsPlayer, Constants::CamRelation camRelation);
	bool getPolarity();
	bool hurtsPlayer();
	bool handle();
	void draw();
	Fixed getx() override;
	Fixed gety() override;
	// speed
	Fixed dx, dy;
	unsigned short* img;
protected:
	bool polarity;
	bool hurtPlayer;
};
