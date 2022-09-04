#include "handlers/DestroyedEnemies.hpp"

#include "utils.hpp"
#include "entities/Enemy.hpp"
#include "helpers/Constants.hpp"
#include "n2DLib/n2DLib_math.h"

DestroyedEnemies::DestroyedEnemies()
{
	clear();
}

DestroyedEnemies::~DestroyedEnemies()
{
}

void DestroyedEnemies::activate(Enemy *e, int c)
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