#include "entities/Bullet.hpp"

#include <n2DLib/n2DLib_math.h>

#include "GameSystems.hpp"
#include "utils.hpp"
#include "handlers/DrawingCandidates.hpp"
#include "helpers/Constants.hpp"

Bullet::Bullet() : Entity()
{
}

void Bullet::activate(Fixed _x, Fixed _y, Fixed _dx, Fixed _dy, LUTs::BaseImageId imgId, bool _p, bool _h, Constants::CamRelation _camRel)
{
	polarity = _p;
	hurtPlayer = _h;
	camRelation = _camRel;
	x = _x;
	y = _y;
	dx = _dx;
	dy = _dy;
	img = LUTs::baseImage(imgId, _p ? 1 : 0);

	Entity::activate();
}

bool Bullet::getPolarity() const
{
	return polarity;
}

bool Bullet::hurtsPlayer() const
{
	return hurtPlayer;
}

bool Bullet::handle()
{
	x += dx;
	y += dy;
				
	// Bullets have a threshold of 5
	return getx() + itofix(img[0] / 2) < itofix(-5) || getx() - itofix(img[0] / 2) > itofix(324) || gety() + itofix(img[1] / 2) < itofix(-5) || gety() - itofix(img[1] / 2) > itofix(244);
}

void Bullet::draw() const
{
	static Rect br;
	br.x = fixtoi(x);
	br.y = fixtoi(y);
	GS->DC->add(img, &br, false, camRelation);
}

Fixed Bullet::getx() const
{
	return fToScreenX(Entity::getx(), camRelation);
}

Fixed Bullet::gety() const
{
	return fToScreenY(Entity::gety(), camRelation);
}