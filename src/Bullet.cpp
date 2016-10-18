#include "common.h"

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

void Bullet::activate(Fixed _x, Fixed _y, Fixed _a, Fixed _r, int imgID, bool _p, bool _h, int _camRel)
{
	active = true;
	polarity = _p;
	hurtPlayer = _h;
	camRel = _camRel;
	x = _x;
	y = _y;
	dx = fixmul(fixcos(_a), _r);
	dy = fixmul(fixsin(_a), _r);
	img = image_entries[imgID + (_p ? 1 : 0)];
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
	DC->add(img, &br, false, camRel);
}

Fixed Bullet::getx()
{
	return fToScreenX(x, camRel);
}

Fixed Bullet::gety()
{
	return fToScreenY(y, camRel);
}