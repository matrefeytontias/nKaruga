#include "entities/Homing.hpp"

#include <n2DLib/n2DLib.h>
#include <n2DLib/n2DLib_math.h>

#include "GameSystems.hpp"
#include "entities/Player.hpp"
#include "handlers/DrawingCandidates.hpp"
#include "helpers/Constants.hpp"

Homing::Homing() : Bullet()
{
	active = false;
	hurtPlayer = true;
}

Homing::~Homing()
{
	
}

void Homing::activate(Fixed _x, Fixed _y, Fixed initialAngle, const Player *_target, bool _p)
{
	x = _x;
	y = _y;
	angle = initialAngle;
	target = _target;
	polarity = _p;
	cachedImg = polarity ? LUTs::BaseImageId::ENEMY_HOMING_BULLET_SHADOW : LUTs::BaseImageId::ENEMY_HOMING_BULLET_LIGHT;
	
	aimTimer = 96;
	active = true;
	for(int i = 0; i < HOMING_TRAILING; i++)
	{
		previousX[i] = x;
		previousY[i] = y;
	}
}

#define HOMING_SPEED 384

bool Homing::handle()
{
	for(int i = HOMING_TRAILING - 1; i > 0; i--)
	{
		previousX[i] = previousX[i - 1];
		previousY[i] = previousY[i - 1];
	}
	previousX[0] = x;
	previousY[0] = y;
	
	x += fixmul(fixcos(angle), HOMING_SPEED);
	y += fixmul(fixsin(angle), HOMING_SPEED);
	
	if(aimTimer)
	{
		if(((angle - angleToEntity(target)) & 0xff) > 128)
			angle++;
		else
			angle--;
		angle &= 0xff;
		aimTimer--;
	}
	
	// As for bullets, homings have a threshold of 30
	return (x + itofix(8) < itofix(-30) || x - itofix(8) > itofix(349) || y + itofix(8) < itofix(-30) || y - itofix(8) > itofix(279));
}

void Homing::draw() const
{
	Rect r;
	
	r.x = fixtoi(x);
	r.y = fixtoi(y);
	
	GS->DC->add(LUTs::baseImage(cachedImg), &r, false, Constants::CamRelation::DEFAULT);
	
	for(int i = 0; i < HOMING_TRAILING; i++)
	{
		r.x = fixtoi(previousX[i]);
		r.y = fixtoi(previousY[i]);
		n2D_fillCircle(r.x, r.y, 8 - i / 2, polarity ? 0xfb0c : 0x65bf);
	}
}
