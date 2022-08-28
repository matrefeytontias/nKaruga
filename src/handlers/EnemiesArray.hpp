#pragma once

#include "entities/Enemy.hpp"
#include "handlers/DestroyedEnemies.hpp"
#include "graphics/ExplosionAnim.hpp"

class EnemiesArray
{
public:
	EnemiesArray();
	~EnemiesArray();
	Enemy* add(int x, int y, int HP, int shipImgID, int callbackID, int waveIndex, bool polarity, bool hasRotation, int firebackAmount, bool ghost, int type);
	void handle();
	void handleExplosions();
	void resetEnemyCounter();
	void destroyAllEnemies();
	bool enemiesKilled();
	Enemy data[MAX_ENEMY];
	DestroyedEnemies deadEnemies;
private:
	ExplosionAnim explosionsAnims[MAX_ENEMY];
	int currentExplosion;
	int currentEnemy;
};
