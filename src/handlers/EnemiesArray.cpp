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

// TODO : LUTs::BaseImageId shipImgId
// TODO : LUTs::EnemyPatternId patternId
// TODO : Constants::EnemyType type
Enemy* EnemiesArray::add(int x, int y, int HP, int shipImgId, int patternId, int waveIndex, bool polarity, bool hasRotation, int firebackAmount, bool ghost, int type)
{
	// Keep props active
	while (data[currentEnemy].isActive() && data[currentEnemy].isProp())
	{
		currentEnemy++;
		currentEnemy %= Constants::MAX_ENEMY;
	}
	data[currentEnemy].activate(x, y, HP, static_cast<LUTs::BaseImageId>(shipImgId), static_cast<LUTs::EnemyPatternId>(patternId), waveIndex, polarity, hasRotation, firebackAmount, ghost, type);
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
			explosionsAnims[currentExplosion].activate(iToScreenX(fixtoi(data[i].getx()), data[i].getCamRel()),
													   iToScreenY(fixtoi(data[i].gety()), data[i].getCamRel()),
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