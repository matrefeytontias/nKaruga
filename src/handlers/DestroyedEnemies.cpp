#include "handlers/DestroyedEnemies.hpp"

#include <n2DLib/n2DLib_math.h>

#include "utils.hpp"
#include "entities/Enemy.hpp"
#include "helpers/Constants.hpp"

DestroyedEnemies::DestroyedEnemies()
{
	clear();
}

void DestroyedEnemies::activate(const Enemy *e, int c)
{
	x[c] = iToScreenX(fixtoi(e->getx()), e->getCamRelation());
	y[c] = iToScreenY(fixtoi(e->gety()), e->getCamRelation());
	relevant[c] = true;
}

void DestroyedEnemies::clear()
{
	for(int i = 0; i < Constants::MAX_ENEMY; i++)
		relevant[i] = false;
}