#pragma once

#include "entities/Bullet.hpp"
#include "entities/Homing.hpp"
#include "entities/Laser.hpp"
#include "entities/PowerFragment.hpp"
#include "helpers/Constants.hpp"

class Enemy;
class Player;

// BulletArray
class BulletArray
{
public:
	BulletArray();
	~BulletArray();
	void handle();
	void add(Fixed x, Fixed y, Fixed a, Fixed r, int imageID, bool polarity, bool hurtsPlayer, int camRel);
	void add_fragment(Fixed x, Fixed y, Fixed initialAngle, Player* target, bool polarity, bool hurtsPlayer);
	void add_homing(Fixed x, Fixed y, Fixed initialAngle, Player* target, bool polarity);
	void fire_laser(Enemy* origin, bool polarity, Fixed angleOffset);
	void stop_laser(Enemy* origin);
	void clear();
private:
	void deactivate(int offset, bool playSound);
	void deactivate_fragment(int offset);
	void deactivate_homing(int offset);
	Bullet data[Constants::MAX_BULLET];
	PowerFragment data_fragment[Constants::MAX_FRAGMENT];
	Homing data_homing[Constants::MAX_HOMING];
	Laser data_laser[Constants::MAX_LASER];
	// keep track of current bullet ...
	int bulletCount;
	// ... power fragment ...
	int fragmentCount;
	// ... homing ...
	int homingCount;
	// ... and laser
	int laserCount;
};
