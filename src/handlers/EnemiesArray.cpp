#include "handlers/EnemiesArray.hpp"

#include "utils.hpp"
#include "helpers/Constants.hpp"
#include "n2DLib/n2DLib_math.h"

EnemiesArray::EnemiesArray()
{
	currentEnemy = 0;
	currentExplosion = 0;
}

EnemiesArray::~EnemiesArray()
{
	
}

Enemy* EnemiesArray::add(int x, int y, int HP, LUTs::BaseImageId shipImgId, LUTs::EnemyPatternId patternId, int waveIndex, bool polarity, bool hasRotation, int firebackAmount, bool ghost, Constants::EnemyType type)
{
	// Keep props active
	while (data[currentEnemy].isActive() && data[currentEnemy].isProp())
	{
		currentEnemy++;
		currentEnemy %= Constants::MAX_ENEMY;
	}
	data[currentEnemy].activate(x, y, HP, shipImgId, patternId, waveIndex, polarity, hasRotation, firebackAmount, ghost, type);
	Enemy *r = &data[currentEnemy];
	currentEnemy++;
	currentEnemy %= Constants::MAX_ENEMY;
	return r;
}

void EnemiesArray::handle()
{
	for(int i = 0; i < Constants::MAX_ENEMY; i++)
	{
		GS->killedThisFrame[i] = -1;
		if(data[i].diedThisFrame)
		{
			GS->killedThisFrame[i] = data[i].getPolarity();
			deadEnemies.activate(&data[i], i);
			explosionsAnims[currentExplosion].activate(iToScreenX(fixtoi(data[i].getx()), data[i].getCamRelation()),
													   iToScreenY(fixtoi(data[i].gety()), data[i].getCamRelation()),
													   data[i].getPolarity());
			currentExplosion++;
			currentExplosion %= Constants::MAX_ENEMY;
		}
		data[i].handle();
	}
}

void EnemiesArray::handleExplosions()
{
	for(int i = 0; i < Constants::MAX_ENEMY; i++)
		if(explosionsAnims[i].isActive())
			explosionsAnims[i].handle();
}

void EnemiesArray::resetEnemyCounter()
{
	currentEnemy = 0;
}

void EnemiesArray::destroyAllEnemies()
{
	for(int i = 0; i < Constants::MAX_ENEMY; i++)
		if(data[i].isActive())
			data[i].damage(data[i].getPolarity(), 9999);
}

bool EnemiesArray::enemiesKilled()
{
	for (int i = 0; i < Constants::MAX_ENEMY; i++)
	{
		if (data[i].isActive() && !data[i].isProp())
			return false;
	}
	return true;
}