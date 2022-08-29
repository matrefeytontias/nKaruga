#include "handlers/DestroyedEnemies.hpp"

#include "utils.hpp"
#include "helpers/Constants.hpp"
#include "helpers/math.hpp"

DestroyedEnemies::DestroyedEnemies()
{
	clear();
}

DestroyedEnemies::~DestroyedEnemies()
{
}

void DestroyedEnemies::activate(Enemy *e, int c)
{
	x[c] = iToScreenX(fixtoi(e->getx()), e->getCamRel());
	y[c] = iToScreenY(fixtoi(e->gety()), e->getCamRel());
	relevant[c] = true;
}

void DestroyedEnemies::clear()
{
	for(int i = 0; i < Constants::MAX_ENEMY; i++)
		relevant[i] = false;
}