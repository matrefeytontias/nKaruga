#include "entities/BossEnemy.hpp"

#include <n2DLib/n2DLib.h>

#include "level/bossData.h"
#include "level/Level.hpp"

BossData createBossData(int bossID)
{
	BossData result;
	result.id = bossID;
	result.HPperBar = bossHPperBar[bossID];
	result.patternsNb = bossPatternsNb[bossID];
	result.HPperPattern = bossHPperPat[bossID];
	result.timeoutPerPattern = bossTimeoutPerPat[bossID];
	result.initCallbacks = bossICBdata[bossID];
	result.callback = bossCBdata[bossID];
	result.collisionCallbacks = bossCCBdata[bossID];
	result.distanceCallbacks = bossDCBdata[bossID];
	result.angleCallbacks = bossACBdata[bossID];
	result.playerCollisionCallbacks = bossPCCBdata[bossID];
	return result;
}

BossEnemy::BossEnemy() : Entity()
{
	// Bind "hurtable" to "active"
	hurtable = &active;
	remainingTime = 0;
	isBoss = true;
}

void BossEnemy::activate(BossData *d)
{
	// Do not call Entity::activate as active is bound to hurtable
	// and this gets taken care of later down the line in the behavior
	// code.
	id = d->id;
	HPperBar = d->HPperBar;
	for(int i = 0; i < 32; i++)
		internal[i] = 0;
	patternsNb = d->patternsNb;
	HPperPattern = d->HPperPattern;
	timeoutPerPattern = d->timeoutPerPattern;
	initCallbacks = d->initCallbacks;
	callback = d->callback;
	collisionCallbacks = d->collisionCallbacks;
	distanceCallbacks = d->distanceCallbacks;
	angleCallbacks = d->angleCallbacks;
	playerCollisionCallbacks = d->playerCollisionCallbacks;
	currentPattern = 0;
	// Mark for initialization callback execution
	readyToGo = false;
	initCallbackCalled = false;
	*hurtable = false;
	remainingTime = timeoutPerPattern[0];
	flash = false;
	angle = 0;
	attachedEnemies.clear();
}

// Return 1 if dead
// Handle collision with the player
int BossEnemy::handle()
{
	if(readyToGo)
	{
		int mHP = HP - getHPsum(HPperPattern, currentPattern + 1, patternsNb - 1);
		int color = mHP / HPperBar + 1;
		mHP %= HPperBar;
		int limit = 240 - mHP * 240 / HPperBar;
		n2D_fillRect(316, 0, 4, limit, color_HPbars[color - 1]);
		n2D_fillRect(316, limit, 4, 239 - limit, color_HPbars[color]);
		
		*hurtable = true;
		(callback)(this);

		if (Level::p->isHurtable() && (playerCollisionCallbacks[currentPattern])(this))
			Level::p->hurt();

		// Timer 1 counts the second
		if(!n2D_timerRead(1))
		{
			remainingTime--;
			n2D_timerLoad(1, 1000);
		}
		if(!remainingTime)
			HP = 0;
	}
	else
	{
		*hurtable = false;
		if (initCallbacks[currentPattern]) (initCallbacks[currentPattern])(this);
	}
	
	return HP < 1;
}

void BossEnemy::damage(int amount)
{
	HP -= amount;
	flash = true;
}

// Equivalent to BossEnemy::isActive()
bool BossEnemy::isHurtable() const
{
	return *hurtable;
}

void BossEnemy::makeHurtable()
{
	*hurtable = true;
}

void BossEnemy::setInternal(int offset, int value)
{
	internal[offset] = value;
}

void BossEnemy::incInternal(int offset)
{
	internal[offset]++;
}

void BossEnemy::decInternal(int offset)
{
	internal[offset]--;
}

int BossEnemy::getInternal(int offset) const
{
	return internal[offset];
}

int BossEnemy::getTimeout() const
{
	return remainingTime;
}

Fixed BossEnemy::getDistance(const PowerFragment *pf) const
{
	return (distanceCallbacks[currentPattern])(this, pf);
}

Fixed BossEnemy::getAngle(const PowerFragment *pf) const
{
	return (angleCallbacks[currentPattern])(this, pf);
}
