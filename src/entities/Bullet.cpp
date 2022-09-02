#include "entities/Bullet.hpp"

#include "globals.h"
#include "utils.hpp"
#include "handlers/DrawingCandidates.hpp"
#include "helpers/Constants.hpp"
#include "helpers/math.hpp"

Bullet::Bullet() : Entity()
{
}

Bullet::~Bullet()
{
}

Rect* Bullet::makeRect()
{
	static Rect r;
	r.x = fixtoi(x);
	r.y = fixtoi(y);
	return &r;
}

// TODO : LUTs::BaseImageId imgId
void Bullet::activate(Fixed _x, Fixed _y, Fixed _a, Fixed _r, int imgId, bool _p, bool _h, int _camRel)
{
	active = true;
	polarity = _p;
	hurtPlayer = _h;
	camRelation = _camRel;
	x = _x;
	y = _y;
	dx = fixmul(fixcos(_a), _r);
	dy = fixmul(fixsin(_a), _r);
	img = LUTs::baseImage(static_cast<LUTs::BaseImageId>(imgId), _p ? 1 : 0);
}

bool Bullet::getPolarity()
{
	return polarity;
}

bool Bullet::hurtsPlayer()
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

void Bullet::draw()
{
	static Rect br;
	br.x = fixtoi(x);
	br.y = fixtoi(y);
	DC->add(img, &br, false, camRelation);
}

Fixed Bullet::getx()
{
	return fToScreenX(x, camRelation);
}

Fixed Bullet::gety()
{
	return fToScreenY(y, camRelation);
}