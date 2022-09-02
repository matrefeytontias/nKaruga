#pragma once

#include "types.h"

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
	int getCamRel();
	Fixed angleToEntity(Entity*);
	Fixed angleToXY(Fixed x, Fixed y);
	Fixed distance2ToEntity(Entity*);
	Fixed distance2ToXY(Fixed x, Fixed y);
	bool isBoss;
	bool isEnemy;
	bool waitFrames(int frames);
protected:
	bool active;
	// TODO : Constants::CamRelation camRelation
	int camRelation;
	Fixed x, y;
};
