#pragma once

#include "types.h"
#include "entities/Entity.hpp"
#include "helpers/Constants.hpp"

// Both enemies and player can fire them
class Bullet : public Entity
{
public:
	Bullet();
	void activate(Fixed x, Fixed y, Fixed dx, Fixed dy, LUTs::BaseImageId imageID, bool polarity, bool hurtsPlayer, Constants::CamRelation camRelation);
	bool getPolarity() const;
	bool hurtsPlayer() const;
	bool handle();
	void draw() const;
	Fixed getx() const override;
	Fixed gety() const override;
	// speed
	Fixed dx, dy;
	const unsigned short* img;
protected:
	bool polarity;
	bool hurtPlayer;
};
