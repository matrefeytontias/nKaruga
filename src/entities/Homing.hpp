#pragma once

#include "entities/Bullet.hpp"

class Player;

#define HOMING_TRAILING 15

// Only enemies can fire them
// So hurtPlayer is always true
class Homing : public Bullet
{
public:
	Homing();
	~Homing();
	void activate(Fixed x, Fixed y, Fixed initialAngle, const Player* target, bool polarity);
	bool handle();
	void draw() const;
private:
	const Player* target;
	Fixed previousX[HOMING_TRAILING];
	Fixed previousY[HOMING_TRAILING];
	Fixed angle;
	int aimTimer;
};
