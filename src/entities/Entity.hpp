#pragma once

#include "types.h"
#include "helpers/Constants.hpp"

// TODO : a lot of subclasses take ints where Fixed should be,
// like Enemy::activate and EnemiesArray::add
class Entity
{
public:
	Entity();
	virtual ~Entity() { }
	virtual Fixed getx() const;
	virtual Fixed gety() const;
	void setx(Fixed);
	void sety(Fixed);
	void addx(Fixed);
	void addy(Fixed);
	void subx(Fixed);
	void suby(Fixed);
	bool isActive() const;
	void activate();
	void deactivate();
	Constants::CamRelation getCamRelation() const;
	Fixed angleToEntity(const Entity*) const;
	Fixed angleToXY(Fixed x, Fixed y) const;
	Fixed distance2ToEntity(const Entity*) const;
	Fixed distance2ToXY(Fixed x, Fixed y) const;
	bool isBoss;
	bool isEnemy;
	bool waitFrames(int frames);
protected:
	bool active;
	Constants::CamRelation camRelation;
	Fixed x, y;
	// Used in waitFrames.
	int waitTimer;
};
