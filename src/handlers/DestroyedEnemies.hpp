#pragma once

#include "GameSystems.hpp"
#include "helpers/Constants.hpp"

class Enemy;

// Used to hold information on killed enemies in order to get the position for ChainNotifs
class DestroyedEnemies
{
public:
	DestroyedEnemies();
	~DestroyedEnemies();
	void activate(Enemy* origin, int offset);
	void clear();
	int x[Constants::MAX_ENEMY];
	int y[Constants::MAX_ENEMY];
	bool relevant[Constants::MAX_ENEMY];
};
