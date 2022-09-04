#include "utils.hpp"

#include <SDL2/SDL.h>

#include "GameSystems.hpp"
#include "handlers/DrawingCandidates.hpp"
#include "level/Level.hpp"
#include "n2DLib/n2DLib.h"
#include "n2DLib/n2DLib_math.h"

int distance(int x1, int y1, int x2, int y2)
{
	return sq(x1 - x2) + sq(y1 - y2);
}

Enemy* findNearestEnemy(Fixed x, Fixed y)
{
	static Enemy* nearest;
	static Enemy* ce;
	
	// Find first if there are any living enemy
	bool hasLivingEnemy = false;
	
	for(int i = 0; i < Constants::MAX_ENEMY; i++)
	{
		ce = &Level::enemiesArray->data[i];
		if(ce->isActive() && ce->isDamageable())
		{
			hasLivingEnemy = true;
			nearest = ce;
			break;
		}
	}
	
	// If not, do nothing
	if(!hasLivingEnemy)
		return NULL;
	else
	{
		// If yes, find the actual nearest enemy
		int lastDistance = distance(fixtoi(nearest->getx()), fixtoi(nearest->gety()), fixtoi(x), fixtoi(y));
		int concurrentDistance;
		
		for(int i = 0; i < Constants::MAX_ENEMY; i++)
		{
			ce = &Level::enemiesArray->data[i];
			if(ce->isActive() && ce->isDamageable())
			{
				concurrentDistance = distance(iToScreenX(fixtoi(ce->getx()), ce->getCamRel()),
											  iToScreenY(fixtoi(ce->gety()), ce->getCamRel()),
											  fixtoi(x), fixtoi(y));
				if(concurrentDistance < lastDistance)
				{
					nearest = ce;
					lastDistance = concurrentDistance;
				}
			}
		}
		return nearest;
	}
}

bool collidePointRect(Fixed px, Fixed py, Fixed x, Fixed y, int w, int h)
{
	return px >= x - itofix(w) / 2 && px < x + itofix(w) / 2 && py >= y - itofix(h) / 2 && py < y + itofix(h) / 2;
}

KeyEvent getk(void)
{
#define setbit(pos, val) k |= val << pos
	KeyEvent k = 0;

	setbit(0, isKeyPressed(GP->keys.down));
	setbit(1, isKeyPressed(GP->keys.left));
	setbit(2, isKeyPressed(GP->keys.right));
	setbit(3, isKeyPressed(GP->keys.up));
	setbit(4, isKeyPressed(GP->keys.fire));
	setbit(5, isKeyPressed(GP->keys.polarity));
	setbit(6, isKeyPressed(GP->keys.fragment));
	setbit(7, isKeyPressed(SDL_SCANCODE_ESCAPE));
	setbit(8, isKeyPressed(GP->keys.pause));

	return k;
#undef setbit
}

int iToScreenX(int x, int camRelation)
{
	return (camRelation == static_cast<int>(Constants::CamRelation::ABSOLUTE) ? x - GS->DC->cam.absX : (camRelation == static_cast<int>(Constants::CamRelation::RELATIVE) ? x - GS->DC->cam.relX : x));
}
int iToScreenY(int y, int camRelation)
{
	return (camRelation == static_cast<int>(Constants::CamRelation::ABSOLUTE) ? y - GS->DC->cam.absY : (camRelation == static_cast<int>(Constants::CamRelation::RELATIVE) ? y - GS->DC->cam.relY : y));
}
Fixed fToScreenX(Fixed x, int camRelation)
{
	return (camRelation == static_cast<int>(Constants::CamRelation::ABSOLUTE) ? x - itofix(GS->DC->cam.absX) : (camRelation == static_cast<int>(Constants::CamRelation::RELATIVE) ? x - itofix(GS->DC->cam.relX) : x));
}
Fixed fToScreenY(Fixed y, int camRelation)
{
	return (camRelation == static_cast<int>(Constants::CamRelation::ABSOLUTE) ? y - itofix(GS->DC->cam.absY) : (camRelation == static_cast<int>(Constants::CamRelation::RELATIVE) ? y - itofix(GS->DC->cam.relY) : y));
}
