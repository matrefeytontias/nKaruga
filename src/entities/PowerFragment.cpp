#include "entities/PowerFragment.hpp"

#include "GameSystems.hpp"
#include "utils.hpp"
#include "handlers/DrawingCandidates.hpp"
#include "helpers/Constants.hpp"
#include "level/Level.hpp"
#include "n2DLib/n2DLib_math.h"

PowerFragment::PowerFragment() : Bullet()
{
	active = false;
	hasReachedAngle = false;
}

PowerFragment::~PowerFragment()
{
	
}

void PowerFragment::activate(Fixed _x, Fixed _y, Fixed initialAngle, Player *_targetP, bool _p, bool _h)
{
	x = _x;
	y = _y;
	angle = initialAngle;
	targetP = _targetP;
	polarity = _p;
	img = LUTs::baseImage(polarity ? LUTs::BaseImageId::PLAYER_HOMING_BULLET_SHADOW_0 : LUTs::BaseImageId::PLAYER_HOMING_BULLET_LIGHT_0);
	hasReachedAngle = false;
	hurtPlayer = _h;
	if(!hurtPlayer)
	{
		targetE = findNearestEnemy(x, y);
		if(Level::fightingBoss)
			targetB = Level::be;
	}
	
	active = true;
	speed = itofix(2);
	skipPositionRecord = 0;
	for(int i = 0; i < FRAGMENT_TRAILING; i++)
	{
		previousX[i] = x;
		previousY[i] = y;
	}
}

#define MAX_SPEED itofix(12)

bool PowerFragment::handle()
{
	if(!(skipPositionRecord % 2))
	{
		for(int i = FRAGMENT_TRAILING - 1; i > 0; i--)
		{
			previousX[i] = previousX[i - 1];
			previousY[i] = previousY[i - 1];
		}
		previousX[0] = x;
		previousY[0] = y;
	}
	skipPositionRecord++;
	
	x += fixmul(fixcos(angle), speed);
	y += fixmul(fixsin(angle), speed);
	
	// If it has not been done already, go to angle = 192 if targetting enemies or angle = 64 if targetting player
	if(!hasReachedAngle)
	{
		if(hurtPlayer)
		{
			if(((angle - 64) & 0xff) > 128)
				angle += 4;
			else
				angle -= 4;
			angle &= 0xff;
			if(abs(angle - 64) <= 1)
				hasReachedAngle = true;
		}
		else
		{
			if(((angle - 192) & 0xff) > 128)
				angle += 4;
			else
				angle -= 4;
			angle &= 0xff;
			if(abs(angle - 192) <= 3)
				hasReachedAngle = true;
		}
	}
	else
	{
		speed += (speed < MAX_SPEED) * 64;
		Fixed dAngle = -1;
		if(hurtPlayer)
		{
			if(targetP->isHurtable())
				dAngle = angleToEntity(targetP);
		}
		else
		{
			targetE = findNearestEnemy(x, y);
			if(targetE)
			{
				if(!Level::fightingBoss)
				{
					dAngle = angleToEntity(targetE);
				}
				else if(targetB->getDistance(this) < distance(fixtoi(x), fixtoi(y), fixtoi(targetE->getx()), fixtoi(targetE->gety())))
				{
					dAngle = targetB->getAngle(this);
				}
				else
				{
					dAngle = angleToEntity(targetE);
				}
			}
			else if(Level::fightingBoss)
			{
				dAngle = targetB->getAngle(this);
			}
		}
		angle = dAngle > -1 ? dAngle : angle;
	}

	// As for bullets, power fragments have a threshold of 30
	return (x + itofix(4) < itofix(-30) || x > itofix(349) || y + itofix(4) < itofix(-30) || y > itofix(279));
}

void PowerFragment::draw()
{
	static Rect r;
	
	r.x = fixtoi(x);
	r.y = fixtoi(y);
	
	GS->DC->add(img, &r, false, Constants::CamRelation::DEFAULT);
	
	for(int i = 0; i < FRAGMENT_TRAILING; i++)
	{
		r.x = fixtoi(previousX[i]);
		r.y = fixtoi(previousY[i]);
		GS->DC->add(LUTs::baseImage(polarity ? LUTs::BaseImageId::PLAYER_HOMING_BULLET_SHADOW_1 : LUTs::BaseImageId::PLAYER_HOMING_BULLET_LIGHT_1,
								i / ((FRAGMENT_TRAILING + 1) / 2)),
				&r, false, Constants::CamRelation::DEFAULT);
	}
}
