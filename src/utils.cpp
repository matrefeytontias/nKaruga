#include "utils.hpp"

#include <n2DLib/n2DLib.h>
#include <n2DLib/n2DLib_math.h>

#include "GameSystems.hpp"
#include "handlers/DrawingCandidates.hpp"
#include "level/Level.hpp"

unsigned int distance(int x1, int y1, int x2, int y2)
{
	return sq(x1 - x2) + sq(y1 - y2);
}

Enemy* findNearestEnemy(Fixed x, Fixed y)
{
	Enemy* currentEnemy;
	Enemy* nearest = nullptr;
	
	unsigned int lastDistance = -1u;
	unsigned int concurrentDistance;

	for (int i = 0; i < Constants::MAX_ENEMY; i++)
	{
		currentEnemy = &Level::enemiesArray->data[i];
		if (currentEnemy->isActive() && currentEnemy->isDamageable())
		{
			concurrentDistance = distance(iToScreenX(fixtoi(currentEnemy->getx()), currentEnemy->getCamRelation()),
										  iToScreenY(fixtoi(currentEnemy->gety()), currentEnemy->getCamRelation()),
										  fixtoi(x), fixtoi(y));
			if (concurrentDistance < lastDistance)
			{
				nearest = currentEnemy;
				lastDistance = concurrentDistance;
			}
		}
	}
	return nearest;
}

bool collidePointRect(Fixed px, Fixed py, Fixed x, Fixed y, int w, int h)
{
	return px >= x - itofix(w) / 2 && px < x + itofix(w) / 2 && py >= y - itofix(h) / 2 && py < y + itofix(h) / 2;
}

// TODO : figure out a better place for this to go to.
KeyEvent getk(void)
{
	KeyEvent k = 0;

#define setbit(pos, val) k |= val << pos

	setbit(0, n2D_isKeyPressed(GP->keys.down));
	setbit(1, n2D_isKeyPressed(GP->keys.left));
	setbit(2, n2D_isKeyPressed(GP->keys.right));
	setbit(3, n2D_isKeyPressed(GP->keys.up));
	setbit(4, n2D_isKeyPressed(GP->keys.fire));
	setbit(5, n2D_isKeyPressed(GP->keys.polarity));
	setbit(6, n2D_isKeyPressed(GP->keys.fragment));
	// TODO : get rid of this
	setbit(7, 0);
	setbit(8, n2D_isKeyPressed(GP->keys.pause));

#undef setbit

	return k;
}

int iToScreenX(int x, Constants::CamRelation camRelation)
{
	return (camRelation == Constants::CamRelation::ABSOLUTE ? x - GS->DC->cam.absX : (camRelation == Constants::CamRelation::RELATIVE ? x - GS->DC->cam.relX : x));
}
int iToScreenY(int y, Constants::CamRelation camRelation)
{
	return (camRelation == Constants::CamRelation::ABSOLUTE ? y - GS->DC->cam.absY : (camRelation == Constants::CamRelation::RELATIVE ? y - GS->DC->cam.relY : y));
}
Fixed fToScreenX(Fixed x, Constants::CamRelation camRelation)
{
	return (camRelation == Constants::CamRelation::ABSOLUTE ? x - itofix(GS->DC->cam.absX) : (camRelation == Constants::CamRelation::RELATIVE ? x - itofix(GS->DC->cam.relX) : x));
}
Fixed fToScreenY(Fixed y, Constants::CamRelation camRelation)
{
	return (camRelation == Constants::CamRelation::ABSOLUTE ? y - itofix(GS->DC->cam.absY) : (camRelation == Constants::CamRelation::RELATIVE ? y - itofix(GS->DC->cam.relY) : y));
}
