#include "entities/Entity.hpp"

#include "GameSystems.hpp"
#include "utils.hpp"
#include "n2DLib/n2DLib_math.h"

Entity::Entity()
{
	x = y = 0;
	active = false;
	camRelation = static_cast<int>(Constants::CamRelation::DEFAULT);
	isEnemy = false;
	isBoss = false;
}

Entity::~Entity()
{
}

Fixed Entity::getx()
{
	return x;
}

void Entity::setx(Fixed _x)
{
	x = _x;
}

Fixed Entity::gety()
{
	return y;
}

void Entity::sety(Fixed _y)
{
	y = _y;
}

void Entity::addx(Fixed _x)
{
	x += _x;
}

void Entity::subx(Fixed _x)
{
	x -= _x;
}

void Entity::addy(Fixed _y)
{
	y += _y;
}

void Entity::suby(Fixed _y)
{
	y -= _y;
}

bool Entity::isActive()
{
	return active;
}

void Entity::activate()
{
	active = true;
}

void Entity::deactivate()
{
	active = false;
}

int Entity::getCamRel()
{
	return camRelation;
}

Fixed Entity::angleToEntity(Entity *e)
{
	int x1 = fToScreenX(getx(), getCamRel()),
		y1 = fToScreenY(gety(), getCamRel()),
		x2 = fToScreenX(e->getx(), e->getCamRel()),
		y2 = fToScreenY(e->gety(), e->getCamRel());
	return e->isActive() ? (Fixed)(atan2((double)(y2 - y1), (double)(x2 - x1)) * 128. / M_PI) & 0xff : -1;
}

Fixed Entity::angleToXY(Fixed x, Fixed y)
{
	int x1 = fToScreenX(getx(), getCamRel()),
		y1 = fToScreenY(gety(), getCamRel());
	return (Fixed)(atan2((double)(y - y1), (double)(x - x1)) * 128. / M_PI) & 0xff;
}

Fixed Entity::distance2ToEntity(Entity *e)
{
	Fixed _x = e->getx() - getx(), _y = e->gety() - gety();
	return fixmul(_x, _x) + fixmul(_y, _y);
}

Fixed Entity::distance2ToXY(Fixed _x, Fixed _y)
{
	Fixed __x = _x - getx(), __y = _y - gety();
	return fixmul(__x, __x) + fixmul(__y, __y);
}

// Returns true when the waiting is done
bool Entity::waitFrames(int frames)
{
	static int base = -1;
	if (base == -1) base = GS->chapterTimer;
	bool r = GS->chapterTimer - base >= frames;
	if (r) base = -1;
	return r;
}