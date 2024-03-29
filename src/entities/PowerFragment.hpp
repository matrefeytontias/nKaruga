#pragma once

#include "types.h"
#include "entities/Bullet.hpp"

class Enemy;
class Player;
class BossEnemy;

#define FRAGMENT_TRAILING 7

// Both enemies and player can fire them
class PowerFragment : public Bullet
{
public:
	PowerFragment();
	~PowerFragment();
	void activate(Fixed x, Fixed y, Fixed initialAngle, Player* target, bool polarity, bool hurtsPlayer);
	bool handle();
	void draw();
	// Polar coordinates of speed
	Fixed speed;
	Fixed angle;
	Enemy* targetE;
	Player* targetP;
	BossEnemy* targetB;
private:
	Fixed previousX[FRAGMENT_TRAILING];
	Fixed previousY[FRAGMENT_TRAILING];
	// Determine if the initial angle has been reached once already
	bool hasReachedAngle;
	int skipPositionRecord;
};