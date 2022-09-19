#pragma once

#include "entities/Enemy.hpp"
#include "handlers/DestroyedEnemies.hpp"
#include "helpers/Constants.hpp"
#include "graphics/ExplosionAnim.hpp"

class EnemiesArray
{
public:
	EnemiesArray();
	Enemy* add(int x, int y, int HP, LUTs::BaseImageId shipImgId, LUTs::EnemyPatternId patternId, int waveIndex, bool polarity, bool hasRotation, int firebackAmount, bool ghost, Constants::EnemyType type);
	void handle();
	void handleExplosions();
	void resetEnemyCounter();
	void destroyAllEnemies();
	bool enemiesKilled() const;
	Enemy data[Constants::MAX_ENEMY];
	DestroyedEnemies deadEnemies;
private:
	ExplosionAnim explosionsAnims[Constants::MAX_ENEMY];
	int currentExplosion;
	int currentEnemy;
};
