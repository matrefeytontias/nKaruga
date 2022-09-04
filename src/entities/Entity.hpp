#pragma once

#include "types.h"
#include "helpers/Constants.hpp"

class Entity
{
public:
	Entity();
	~Entity();
	virtual Fixed getx();
	virtual Fixed gety();
	void setx(Fixed);
	void sety(Fixed);
	void addx(Fixed);
	void subx(Fixed);
	void addy(Fixed);
	void suby(Fixed);
	bool isActive();
	void activate();
	void deactivate();
	Constants::CamRelation getCamRelation();
	Fixed angleToEntity(Entity*);
	Fixed angleToXY(Fixed x, Fixed y);
	Fixed distance2ToEntity(Entity*);
	Fixed distance2ToXY(Fixed x, Fixed y);
	bool isBoss;
	bool isEnemy;
	bool waitFrames(int frames);
protected:
	bool active;
	Constants::CamRelation camRelation;
	Fixed x, y;
};
