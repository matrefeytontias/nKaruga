#include "entities/Entity.hpp"

#include <n2DLib/n2DLib_math.h>

#include "GameSystems.hpp"
#include "utils.hpp"

Entity::Entity()
{
	x = y = 0;
	active = false;
	camRelation = Constants::CamRelation::DEFAULT;
	isEnemy = false;
	isBoss = false;
}

Fixed Entity::getx() const
{
	return x;
}

void Entity::setx(Fixed _x)
{
	x = _x;
}

Fixed Entity::gety() const
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

void Entity::addy(Fixed _y)
{
	y += _y;
}

void Entity::subx(Fixed _x)
{
	x -= _x;
}

void Entity::suby(Fixed _y)
{
	y -= _y;
}

bool Entity::isActive() const
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

Constants::CamRelation Entity::getCamRelation() const
{
	return camRelation;
}

// TODO : figure out fixed-point versions ? not sure it's worth it
Fixed Entity::angleToEntity(const Entity *e) const
{
	int x1 = fToScreenX(getx(), getCamRelation()),
		y1 = fToScreenY(gety(), getCamRelation()),
		x2 = fToScreenX(e->getx(), e->getCamRelation()),
		y2 = fToScreenY(e->gety(), e->getCamRelation());
	return e->isActive() ? (Fixed)(atan2f((float)(y2 - y1), (float)(x2 - x1)) * 128.f / M_PI) & 0xff : -1;
}

Fixed Entity::angleToXY(Fixed x, Fixed y) const
{
	int x1 = fToScreenX(getx(), getCamRelation()),
		y1 = fToScreenY(gety(), getCamRelation());
	return (Fixed)(atan2f((float)(y - y1), (float)(x - x1)) * 128.f / M_PI) & 0xff;
}

Fixed Entity::distance2ToEntity(const Entity *e) const
{
	Fixed _x = e->getx() - getx(), _y = e->gety() - gety();
	return fixmul(_x, _x) + fixmul(_y, _y);
}

Fixed Entity::distance2ToXY(Fixed _x, Fixed _y) const
{
	Fixed __x = _x - getx(), __y = _y - gety();
	return fixmul(__x, __x) + fixmul(__y, __y);
}

// Returns true when the waiting is done
bool Entity::waitFrames(int frames)
{
	if (waitTimer == -1) waitTimer = GS->chapterTimer;
	bool r = GS->chapterTimer - waitTimer >= frames;
	if (r) waitTimer = -1;
	return r;
}
