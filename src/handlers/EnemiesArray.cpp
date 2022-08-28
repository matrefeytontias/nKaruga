#include "handlers/EnemiesArray.hpp"

#include "fixmath.h"
#include "utils.hpp"

EnemiesArray::EnemiesArray()
{
	currentEnemy = 0;
	currentExplosion = 0;
}

EnemiesArray::~EnemiesArray()
{
	
}

Enemy* EnemiesArray::add(int x, int y, int HP, int shipImgID, int callbackID, int waveIndex, bool polarity, bool hasRotation, int firebackAmount, bool ghost, int type)
{
	// Keep props active
	while (data[currentEnemy].isActive() && data[currentEnemy].isProp())
	{
		currentEnemy++;
		currentEnemy %= MAX_ENEMY;
	}
	data[currentEnemy].activate(x, y, HP, shipImgID, callbackID, waveIndex, polarity, hasRotation, firebackAmount, ghost, type);
	Enemy *r = &data[currentEnemy];
	currentEnemy++;
	currentEnemy %= MAX_ENEMY;
	return r;
}

void EnemiesArray::handle()
{
	for(int i = 0; i < MAX_ENEMY; i++)
	{
		G_killedThisFrame[i] = -1;
		if(data[i].diedThisFrame)
		{
			G_killedThisFrame[i] = data[i].getPolarity();
			deadEnemies.activate(&data[i], i);
			explosionsAnims[currentExplosion].activate(iToScreenX(fixtoi(data[i].getx()), data[i].getCamRel()),
													   iToScreenY(fixtoi(data[i].gety()), data[i].getCamRel()),
													   data[i].getPolarity());
			currentExplosion++;
			currentExplosion %= MAX_ENEMY;
		}
		data[i].handle();
	}
}

void EnemiesArray::handleExplosions()
{
	for(int i = 0; i < MAX_ENEMY; i++)
		explosionsAnims[i].handle();
}

void EnemiesArray::resetEnemyCounter()
{
	currentEnemy = 0;
}

void EnemiesArray::destroyAllEnemies()
{
	for(int i = 0; i < MAX_ENEMY; i++)
		if(data[i].isActive())
			data[i].damage(data[i].getPolarity(), 9999);
}

bool EnemiesArray::enemiesKilled()
{
	for (int i = 0; i < MAX_ENEMY; i++)
	{
		if (data[i].isActive() && !data[i].isProp())
			return false;
	}
	return true;
}