#pragma once

#include <vector>

#include "types.h"
#include "entities/Entity.hpp"
#include "helpers/Constants.hpp"

class BossEnemy;
class Bullet;
class Enemy;
class PowerFragment;

/* Bosses' data format
 * - Number of patterns
 * - For each pattern :
 *   - Amount of HP
 *   - Collision callback ID (called to test collision between the boss and bullets, it can change depending on the boss's state)
 * - Array of every needed images
 *
 * The callback is supposed to handle all patterns by testing the amount of HP.
 * Exceptionally, drawing should be taken care of in the callback using drawing candidates since bosses have complex graphical behaviors.
 */

 // Initialization callback, handling pre-battle cinematics and such
 // Inputs :
 // - boss enemy to initialize
 // Output : none
typedef void (*boss_icb)(BossEnemy*);
// Behaviour callback, handling the boss's patterns during battle
// Inputs :
// - boss enemy to handle
// - player object for aiming
// - bullet array for firing bullets
// Output : none
typedef void (*boss_cb)(BossEnemy*);
// Collision callback, handling whether or not the boss is being hit by a bullet
// Inputs :
// - boss enemy to test
// - bullet object
// - amount of damage to deal if the boss is hit
// Output : amount of damage dealt, 0 for no collision, negative for collision with no damage
typedef int (*boss_ccb)(BossEnemy*, Bullet*, int);
// Player collision callback, handling whether or not the player destroys (him|her)self on the boss
// Inputs :
// - boss enemy to test
// - player object
// Output : boolean
typedef bool (*boss_pccb)(BossEnemy*);
// Distance callback, handling distance calcualtion for when the boss is being aimed by power fragments
// Inputs :
// - boss enemy to test
// - power fragment object
// Output : distance to boss
typedef Fixed(*boss_dcb)(BossEnemy*, PowerFragment*);
// Angle callback, handling angle calculation for when the boss is being aimed by power fragments
// Inputs :
// - boss enemy to test
// - power fragment object
// Output : angle to boss
typedef Fixed(*boss_acb)(BossEnemy*, PowerFragment*);

typedef struct
{
	int id;
	int HPperBar;
	boss_cb callback;
	// Every following member contains this amount of data, because it's pattern-specific
	int patternsNb;
	// Self-explanatory
	int* HPperPattern;
	int* timeoutPerPattern;
	boss_icb* initCallbacks;
	boss_ccb* collisionCallbacks;
	boss_dcb* distanceCallbacks;
	boss_acb* angleCallbacks;
	boss_pccb* playerCollisionCallbacks;
} BossData;

BossData createBossData(int bossID);

// The really big bad one
class BossEnemy : public Entity
{
public:
	BossEnemy();
	void activate(BossData* d); // drawing is taken care of by the callback code
	int handle();
	void damage(int amount);
	bool isHurtable();
	void makeHurtable();
	void setInternal(int offset, int value);
	void incInternal(int offset);
	void decInternal(int offset);
	int getInternal(int offset);
	int getTimeout();
	Fixed getDistance(PowerFragment*);
	Fixed getAngle(PowerFragment*);
	int id;
	uint16_t *bodyImg; // image of the boss' body
	Fixed angle;
	bool flash;
	int HP;
	int maxHP;
	int HPperBar;
	int currentPattern;
	int lastPattern;
	int* HPperPattern;
	int* timeoutPerPattern;
	int patternsNb;
	bool readyToGo;
	bool initCallbackCalled;
	boss_icb* initCallbacks;
	boss_cb callback;
	boss_ccb* collisionCallbacks;
	boss_dcb* distanceCallbacks;
	boss_acb* angleCallbacks;
	boss_pccb* playerCollisionCallbacks;
	// Used to save attached enemies
	std::vector<Enemy*> attachedEnemies;
private:
	int remainingTime;
	bool* hurtable;
	// LOTS of internal registers
	int internal[32];
};
