#pragma once

#include "common.h"
#include "graphics/ExplosionEffect.hpp"

#define BOSS_NB 2
#define MAX_PATTERNS_PER_BOSS 3

// Colors for bosses' HP bars
// from low to high HP
// red (no HP), orange, yellow, green-yellow, dark green, green, cyan, navy, blue, purple, magenta, olive, dark gray, white
unsigned short color_HPbars[] = { 0xf800, 0xfd20, 0xffe0, 0xafe5, 0x03e0, 0x07e0, 0x07ff, 0x000f, 0x001f, 0x780f, 0xf81f, 0x7be0, 0x7bef, 0xffff };

// Case-specific enums

/*
 * Boss 1
 */

// Joints
enum
{
	boss1_joint_leftarm_armed,
	boss1_joint_rightarm_armed1,
	boss1_joint_leftarm_nonarmed,
	boss1_joint_rightarm_armed2,
	boss1_joint_rightarm_nonarmed,
	NB_JOINT_BOSS1
};

/*
 * Boss 2
 */

// Joints
enum
{
	boss2_joint_leftUpperArm,
	boss2_joint_rightUpperArm,
	boss2_joint_leftArm,
	boss2_joint_rightArm,
	boss2_joint_leftWing,
	boss2_joint_rightWing,
	boss2_joint_leftShield,
	boss2_joint_rightShield,
	boss2_joint_rails,
	NB_JOINT_BOSS2
};

// Ghost enemies used as joints
enum
{
	boss2_ghost_leftWeakPoint,
	boss2_ghost_rightWeakPoint,
	boss2_ghost_leftShield,
	boss2_ghost_rightShield,
	boss2_ghost_leftUpperArm,
	boss2_ghost_rightUpperArm,
	boss2_ghost_leftArm,
	boss2_ghost_rightArm,
	boss2_ghost_rails,
	NB_GHOSTS_BOSS2
};

/* ####################
 * 
 * DATA
 *
 * ####################
 */

// source(x, y) , destination(x, y)
int boss1_jointData[NB_JOINT_BOSS1][4] = {
	{ 11, 4, 38, 22 },
	{ 11, 8, 74, 22 },
	{ 22, 8, 38, 23 },
	{ 24, 22, 73, 23 },
	{ 2, 8, 72, 22 }
};

int boss2_jointData[NB_JOINT_BOSS2][4] = {
	{ 35, 3, 19, 71 }, // left upper arm to body
	{ 4, 3, 123, 72 }, // right upper arm to body
	{ 46, 13, 8, 23 }, // left arm to left upper arm
	{ 15, 14, 32, 23 }, // right arm to right upper arm
	{ 120, 32, 11, 68 }, // left wing to body
	{ 17, 32, 130, 68 }, // right wing to body
	{ 19, 12, 49, 113 }, // left shield to body (the two last numbers give the location of the respective weak point)
	{ 19, 12, 99, 113 }, // right shield to body (same remark)
	{ 38, 0, 71, 102 } // rails to body
};

// General data

int bossHPperBar[BOSS_NB] = { 400, 300 };

int bossPatternsNb[BOSS_NB] = { 3, 1 };

int bossHPperPat[BOSS_NB][MAX_PATTERNS_PER_BOSS] = {
	{ 400 * 3 / 4, 400, 400 * 3 },
	{ 0, 0, 0 } // handled in the behaviour
};

int bossTimeoutPerPat[BOSS_NB][MAX_PATTERNS_PER_BOSS] = {
	{ 120, 80, 40 },
	{ 120, 0, 0 }
};

/* ####################
 * 
 * UTILS
 *
 * ####################
 */

int getHPsum(int *a, int start, int end)
{
	int total = 0;
	for(int i = start; i <= end; i++)
		total += a[i];
	return total;
}

int getPatternID(BossEnemy *be)
{
	int i;
	int HPdone = -1, timeDone = -1;
	for(i = 0; i < be->patternsNb; i++)
	{
		if(HPdone < 0)
		{
			if(be->HP > getHPsum(be->HPperPattern, i, be->patternsNb - 1))
				HPdone = i - 1;
		}
		if(timeDone < 0)
		{
			if(be->getTimeout() > be->timeoutPerPattern[i])
				timeDone = i - 1;
		}
		if(!(HPdone < 0 || timeDone < 0))
			break;
	}
	return !(HPdone < 0 || timeDone < 0) ? max(HPdone, timeDone) : i - 1;
}

// Works with every angle as the jointing point is not supposed to rotate at all
// x, y = position of jointing point on-screen
// w, h = position of jointing point in the jointed sprite
// To use with rotating sprites
Rect getJointPoint(BossEnemy *be, int data[][4], int offset)
{
	Rect result;
	result.x = fixtoi(be->getx()) - bossImage_entries[be->body][0] / 2 + data[offset][2];
	result.y = fixtoi(be->gety()) - bossImage_entries[be->body][1] / 2 + data[offset][3];
	result.w = data[offset][0];
	result.h = data[offset][1];
	
	return result;
}

// Gives the on-screen position of the image using the given joint info
Rect getFullJointedPos(Entity *base, int bimgID, int imgID, int data[][4], int offset)
{
	Rect result;
	unsigned short *bimg = bossImage_entries[bimgID], *img = bossImage_entries[imgID];
	result.x = fixtoi(base->getx()) + data[offset][2] - bimg[0] / 2; // calculate the base point first ; the joint point must be right on it on-screen
	result.x += data[offset][0] - img[0] / 2; // superpose the joint point with the base point
	result.y = fixtoi(base->gety()) + data[offset][3] - bimg[1] / 2;
	result.y -= data[offset][1] - img[1] / 2;
	return result;
}

#define attached(x) be->attachedEnemies[x]

void boss2_display(BossEnemy *be)
{
	Rect pos, centerRot;

	// Display the boss
	// In that order : rails, wings, upper arms, arms, shields, body
	// [0] : wings angle
	// [28] : wings image index

	pos = getFullJointedPos(be, be->body, bossImage_LUT_2_rails, boss2_jointData, boss2_joint_rails);
	DC->add(bossImage_entries[bossImage_LUT_2_rails], &pos, false, be->getCamRel());

	pos = getJointPoint(be, boss2_jointData, boss2_joint_leftWing);
	centerRot.x = pos.w;
	centerRot.y = pos.h;
	DC->add(bossImage_entries[bossImage_LUT_2_leftWing + be->getInternal(28)], &pos, &centerRot, be->getInternal(0), false, be->getCamRel());
	pos = getJointPoint(be, boss2_jointData, boss2_joint_rightWing);
	centerRot.x = pos.w;
	centerRot.y = pos.h;
	DC->add(bossImage_entries[bossImage_LUT_2_rightWing + be->getInternal(28)], &pos, &centerRot, be->getInternal(1), false, be->getCamRel());

	pos = getJointPoint(be, boss2_jointData, boss2_joint_leftUpperArm);
	pos.x = fixtoi(attached(boss2_ghost_leftUpperArm)->getx());
	pos.y = fixtoi(attached(boss2_ghost_leftUpperArm)->gety());
	centerRot.x = pos.w;
	centerRot.y = pos.h;
	DC->add(bossImage_entries[bossImage_LUT_2_leftUpperArm], &pos, &centerRot, attached(boss2_ghost_leftUpperArm)->rotationAngle, false, be->getCamRel());
	pos = getJointPoint(be, boss2_jointData, boss2_joint_rightUpperArm);
	pos.x = fixtoi(attached(boss2_ghost_rightUpperArm)->getx());
	pos.y = fixtoi(attached(boss2_ghost_rightUpperArm)->gety());
	centerRot.x = pos.w;
	centerRot.y = pos.h;
	DC->add(bossImage_entries[bossImage_LUT_2_rightUpperArm], &pos, &centerRot, attached(boss2_ghost_rightUpperArm)->rotationAngle, false, be->getCamRel());
	
	pos.x = fixtoi(attached(boss2_ghost_leftArm)->getx());
	pos.y = fixtoi(attached(boss2_ghost_leftArm)->gety());
	DC->add(bossImage_entries[bossImage_LUT_2_leftArm], &pos, false, be->getCamRel());
	pos.x = fixtoi(attached(boss2_ghost_rightArm)->getx());
	pos.y = fixtoi(attached(boss2_ghost_rightArm)->gety());
	DC->add(bossImage_entries[bossImage_LUT_2_rightArm], &pos, false, be->getCamRel());

	pos.x = fixtoi(attached(boss2_ghost_leftShield)->getx());
	pos.y = fixtoi(attached(boss2_ghost_leftShield)->gety());
	DC->add(bossImage_entries[bossImage_LUT_2_leftShield], &pos, NULL, attached(boss2_ghost_leftShield)->rotationAngle, false, be->getCamRel());
	pos.x = fixtoi(attached(boss2_ghost_rightShield)->getx());
	pos.y = fixtoi(attached(boss2_ghost_rightShield)->gety());
	DC->add(bossImage_entries[bossImage_LUT_2_rightShield], &pos, NULL, attached(boss2_ghost_rightShield)->rotationAngle, false, be->getCamRel());

	pos.x = fixtoi(be->getx());
	pos.y = fixtoi(be->gety());
	DC->add(bossImage_entries[bossImage_LUT_2_body], &pos, false, be->getCamRel());
}

#undef attached

/* ####################
 * 
 * BOSS PROGRAMMING
 *
 * ####################
 */

// [x]: usage
// refers to a register number and the usage that's made of it inside the block where the comment is put

/*
 * ######
 * BOSS 1
 * ######
 */

/*
 * ########################
 * Initialization callbacks
 * ########################
 */
 
/*
 * Boss 1
 */

void boss1_icb1(BossEnemy *be)
{
	const int h = bossImage_entries[bossImage_LUT_1_body][1];
	if(!be->initCallbackCalled)
	{
		be->body = bossImage_LUT_1_body;
		be->setx(itofix(160));
		be->sety(-itofix(h) / 2);
		be->maxHP = be->HP = getHPsum(be->HPperPattern, 0, be->patternsNb - 1);
		be->setInternal(1, -32);
		be->angle = 0;
		be->initCallbackCalled = true;
		be->setInternal(31, 0);
	}
	if(be->gety() < itofix(h) / 2)
		be->addy(128);
	else
	{
		// [31] : pause timer
		if(be->getInternal(31) < 64)
			be->incInternal(31);
		else
		{
			if(be->getInternal(1))
			{
				// [30] : angle increment timer
				if(!(be->getInternal(30) % 2))
					be->incInternal(1);
				be->incInternal(30);
			}
			else
			{
				be->currentPattern = be->lastPattern = 0;
				be->setInternal(30, 0);
				be->setInternal(31, 0);
				Level::enemiesArray->add(itofix(20), itofix(-20), 100, image_LUT_boss1_enemy_ship_shadow, Pattern_1_boss, 0, SHADOW, true, 0, false, false);
				Level::enemiesArray->add(itofix(300), itofix(-20), 100, image_LUT_boss1_enemy_ship_shadow, Pattern_1_boss, 1, SHADOW, true, 0, false, false);
				be->HP = getHPsum(be->HPperPattern, 0, be->patternsNb - 1);
				be->readyToGo = true;
			}
		}
	}
	Rect pos = { fixtoi(be->getx()), fixtoi(be->gety()), 0, 0 }, centerRot;
	DC->add(bossImage_entries[bossImage_LUT_1_body], &pos, false, CAMREL_RELATIVE);
	
	pos = getJointPoint(be, boss1_jointData, boss1_joint_rightarm_armed1);
	centerRot.x = pos.w;
	centerRot.y = pos.h;
	DC->add(bossImage_entries[bossImage_LUT_1_rightarm_armed1], &pos, &centerRot, be->angle, false, CAMREL_RELATIVE);
	
	// [1]: angle of sword [-32, 0]
	pos = getJointPoint(be, boss1_jointData, boss1_joint_leftarm_armed);
	rotate(pos.x, pos.y, fixtoi(be->getx()), fixtoi(be->gety()), be->angle, &pos);
	centerRot.x = pos.w;
	centerRot.y = pos.h;
	DC->add(bossImage_entries[bossImage_LUT_1_leftarm_armed], &pos, &centerRot, be->getInternal(1) + be->angle, false, CAMREL_RELATIVE);
}

void boss1_icb2(BossEnemy *be)
{
	const int h = bossImage_entries[bossImage_LUT_1_body][1];
	static Fixed dx = 0, dy = 0;
	Rect pos, centerRot;
	if(!be->initCallbackCalled)
	{
		dx = (itofix(160) - be->getx()) / 64;
		dy = (itofix(h / 2) - be->gety()) / 64;
		pos = getJointPoint(be, boss1_jointData, be->currentPattern == 1 ? boss1_joint_leftarm_armed : boss1_joint_rightarm_armed2);
		initExplosionEffect(pos.x, pos.y, 150, 0);
		be->initCallbackCalled = true;
		be->angle = 0;
	}
	updateExplosionEffect();
	
	if(fixtoi(be->getx()) != 160 || fixtoi(be->gety()) != h / 2)
	{
		be->addx(dx);
		be->addy(dy);
	}
	else
	{
		// [31] : pause timer
		if(be->getInternal(31) >= 64)
		{
			be->setInternal(0, 0);
			be->setInternal(31, 0);
			Level::enemiesArray->add(itofix(20), itofix(-20), 100, image_LUT_boss1_enemy_ship_light , Pattern_1_boss, 0, LIGHT, true, 0, false, false);
			Level::enemiesArray->add(itofix(300), itofix(-20), 100, be->currentPattern == 1 ? image_LUT_boss1_enemy_ship_light : image_LUT_boss1_enemy_ship_shadow,
				Pattern_1_boss, 1, be->currentPattern == 1 ? LIGHT : SHADOW, true, 0, false, false);
			be->HP = getHPsum(be->HPperPattern, be->currentPattern, be->patternsNb - 1);
			be->readyToGo = true;
		}
		be->incInternal(31);
	}
		
	pos.x = fixtoi(be->getx());
	pos.y = fixtoi(be->gety());
	DC->add(bossImage_entries[bossImage_LUT_1_body], &pos, false, CAMREL_RELATIVE);
	
	pos = getJointPoint(be, boss1_jointData, be->currentPattern == 1 ? boss1_joint_rightarm_armed1 : boss1_joint_rightarm_nonarmed);
	centerRot.x = pos.w;
	centerRot.y = pos.h;
	DC->add(bossImage_entries[be->currentPattern == 1 ? bossImage_LUT_1_rightarm_armed1 : bossImage_LUT_1_rightarm_nonarmed], &pos, &centerRot, be->angle, false, CAMREL_RELATIVE);
	
	pos = getJointPoint(be, boss1_jointData, boss1_joint_leftarm_nonarmed);
	rotate(pos.x, pos.y, fixtoi(be->getx()), fixtoi(be->gety()), be->angle, &pos);
	centerRot.x = pos.w;
	centerRot.y = pos.h;
	DC->add(bossImage_entries[bossImage_LUT_1_leftarm_nonarmed], &pos, &centerRot, be->angle, false, CAMREL_RELATIVE);
}

/**********
 * Boss 2 *
 **********/

#define attached(x) be->attachedEnemies[x]
#define ANGLE_LEFTWING_UP (-11)
#define ANGLE_LEFTWING_DOWN 46
#define ANGLE_RIGHTWING_UP (-ANGLE_LEFTWING_UP)
#define ANGLE_RIGHTWING_DOWN (-ANGLE_LEFTWING_DOWN)
#define ANGLE_LEFTUPPERARM_UP (-96)
#define ANGLE_LEFTUPPERARM_DOWN 0
#define ANGLE_RIGHTUPPERARM_UP (-ANGLE_LEFTUPPERARM_UP)
#define ANGLE_RIGHTUPPERARM_DOWN (-ANGLE_LEFTUPPERARM_DOWN)

void boss2_icb(BossEnemy *be)
{
	// [0] : left wing angle
	// [1] : right wing angle
	// [2] : shield acceleration
	// [3] : shield movement completion
	// [28] : wings image index
	// [29] : sine movement phase
	Enemy *lg, *rg;
	int w = bossImage_entries[bossImage_LUT_2_body][0], h = bossImage_entries[bossImage_LUT_2_body][1];
	if (!be->initCallbackCalled)
	{
		be->initCallbackCalled = true;
		be->body = bossImage_LUT_2_body;

		be->setx(itofix(160));
		be->sety(-itofix(h));
		// Initialize ghost enemies/joints
		be->attachedEnemies.resize(NB_GHOSTS_BOSS2);
		// Weak point		
		attached(boss2_ghost_leftWeakPoint) = Level::enemiesArray->add(0, 0, be->HPperBar * 2, image_LUT_null, Pattern_2_bossWeakPoint, 0, LIGHT, false, 0, false, TYPE_ENEMY);
		lg = attached(boss2_ghost_leftWeakPoint);
		lg->joint(be, boss2_jointData[boss2_joint_leftShield][2], boss2_jointData[boss2_joint_leftShield][3], 0, 0, -1, -1,
			bossImage_entries[be->body], image_entries[image_LUT_null], false);
		attached(boss2_ghost_rightWeakPoint) = Level::enemiesArray->add(0, 0, be->HPperBar * 2, image_LUT_null, Pattern_2_bossWeakPoint, 0, SHADOW, false, 0, false, TYPE_ENEMY);
		rg = attached(boss2_ghost_rightWeakPoint);
		rg->joint(be, boss2_jointData[boss2_joint_rightShield][2], boss2_jointData[boss2_joint_leftShield][3], 0, 0, -1, -1,
			bossImage_entries[be->body], image_entries[image_LUT_null], false);
		lg->damageable = rg->damageable = false;

		// Shield joint		
		attached(boss2_ghost_leftShield) = Level::enemiesArray->add(0, itofix(60), 1, image_LUT_boss2_leftShield, Pattern_2_bossShield, 0, LIGHT, false, 0, false, TYPE_ENEMY);
		lg = attached(boss2_ghost_leftShield);
		lg->joint(be, boss2_jointData[boss2_joint_leftShield][2], boss2_jointData[boss2_joint_leftShield][3],
			boss2_jointData[boss2_joint_leftShield][0], boss2_jointData[boss2_joint_leftShield][1], -1, -1, bossImage_entries[be->body], image_entries[image_LUT_boss2_leftShield], false);
		attached(boss2_ghost_rightShield) = Level::enemiesArray->add(0, itofix(60), 1, image_LUT_boss2_rightShield, Pattern_2_bossShield, 0, SHADOW, false, 0, false, TYPE_ENEMY);
		rg = attached(boss2_ghost_rightShield);
		rg->joint(be, boss2_jointData[boss2_joint_rightShield][2], boss2_jointData[boss2_joint_rightShield][3],
			boss2_jointData[boss2_joint_rightShield][0], boss2_jointData[boss2_joint_rightShield][1], -1, -1, bossImage_entries[be->body], image_entries[image_LUT_boss2_rightShield], false);
		lg->damageable = rg->damageable = false;
		lg->visible = rg->visible = false;

		// Upper arms joints
		attached(boss2_ghost_leftUpperArm) = Level::enemiesArray->add(0, 0, 1, image_LUT_boss2_leftUpperArm, Pattern_null, 0, LIGHT, true, 0, false, TYPE_ENEMY);
		lg = attached(boss2_ghost_leftUpperArm);
		lg->joint(be, boss2_jointData[boss2_joint_leftUpperArm][2], boss2_jointData[boss2_joint_leftUpperArm][3],
			boss2_jointData[boss2_joint_leftUpperArm][0], boss2_jointData[boss2_joint_leftUpperArm][1], boss2_jointData[boss2_joint_leftUpperArm][0],
			boss2_jointData[boss2_joint_leftUpperArm][1], bossImage_entries[be->body], bossImage_entries[bossImage_LUT_2_leftUpperArm], false);
		attached(boss2_ghost_rightUpperArm) = Level::enemiesArray->add(0, 0, 1, image_LUT_boss2_rightUpperArm, Pattern_null, 0, LIGHT, true, 0, false, TYPE_ENEMY);
		rg = attached(boss2_ghost_rightUpperArm);
		rg->joint(be, boss2_jointData[boss2_joint_rightUpperArm][2], boss2_jointData[boss2_joint_rightUpperArm][3],
			boss2_jointData[boss2_joint_rightUpperArm][0], boss2_jointData[boss2_joint_rightUpperArm][1],
			boss2_jointData[boss2_joint_rightUpperArm][0], boss2_jointData[boss2_joint_rightUpperArm][1], bossImage_entries[be->body],
			bossImage_entries[bossImage_LUT_2_rightUpperArm], false);
		lg->damageable = rg->damageable = false;
		lg->visible = rg->visible = false;

		// Arms joints
		attached(boss2_ghost_leftArm) = Level::enemiesArray->add(0, 0, 1, image_LUT_null, Pattern_null, 0, LIGHT, true, 0, true, TYPE_ENEMY);
		lg = attached(boss2_ghost_leftArm);
		lg->joint(attached(boss2_ghost_leftUpperArm), boss2_jointData[boss2_joint_leftArm][2], boss2_jointData[boss2_joint_leftArm][3],
			boss2_jointData[boss2_joint_leftUpperArm][0], boss2_jointData[boss2_joint_leftUpperArm][1], boss2_jointData[boss2_joint_leftArm][0],
			boss2_jointData[boss2_joint_leftArm][1], -1, -1, bossImage_entries[bossImage_LUT_2_leftUpperArm], bossImage_entries[bossImage_LUT_2_leftArm], false);
		attached(boss2_ghost_rightArm) = Level::enemiesArray->add(0, 0, 1, image_LUT_null, Pattern_null, 0, SHADOW, true, 0, true, TYPE_ENEMY);
		rg = attached(boss2_ghost_rightArm);
		rg->joint(attached(boss2_ghost_rightUpperArm), boss2_jointData[boss2_joint_rightArm][2], boss2_jointData[boss2_joint_rightArm][3],
			boss2_jointData[boss2_joint_rightUpperArm][0], boss2_jointData[boss2_joint_rightUpperArm][1], boss2_jointData[boss2_joint_rightArm][0],
			boss2_jointData[boss2_joint_rightArm][1], -1, -1, bossImage_entries[bossImage_LUT_2_rightUpperArm], bossImage_entries[bossImage_LUT_2_rightArm], false);
		lg->visible = rg->visible = false;

		be->maxHP = be->HP = attached(boss2_ghost_leftWeakPoint)->HP + attached(boss2_ghost_rightWeakPoint)->HP;
		// Set up internals
		//be->setInternal(0, ANGLE_LEFTWING_UP); // approximately 15°
		//be->setInternal(1, ANGLE_RIGHTWING_UP);
		be->setInternal(0, ANGLE_LEFTWING_DOWN); // approximately 65°
		be->setInternal(1, ANGLE_RIGHTWING_DOWN);
		attached(boss2_ghost_leftUpperArm)->rotationAngle = ANGLE_LEFTUPPERARM_UP;
		attached(boss2_ghost_rightUpperArm)->rotationAngle = ANGLE_RIGHTUPPERARM_UP;
		be->setInternal(2, 0);
		be->setInternal(3, 0);
	}
	else
	{		
		// Slowly come from the top of the screen until the boss hits (160, h / 2)
		if (fixtoi(be->gety()) < h / 2)
			be->addy(128);
		else if (be->getInternal(0) > ANGLE_LEFTWING_UP)
		{
			if (be->waitFrames(1))
			{
				be->decInternal(0);
				be->incInternal(1);
			}
		}
		else if (!be->getInternal(3))
		{
			Fixed a = be->getInternal(2);
			Enemy *ls = attached(boss2_ghost_leftShield), *rs = attached(boss2_ghost_rightShield);
			ls->addy(a);
			rs->addy(a);
			if (ls->rawy() < 0)
			{
				ls->sety(0);
				rs->sety(0);
				be->setInternal(2, -a / 2);
				be->setInternal(3, abs(be->getInternal(2)) < itofix(1));
			}
			else
				be->setInternal(2, a - 16);
		}
		else
		{
			be->readyToGo = true;
			attached(boss2_ghost_leftShield)->internal[0] = 1;
			attached(boss2_ghost_rightShield)->internal[0] = 1;
			attached(boss2_ghost_leftWeakPoint)->damageable = true;
			attached(boss2_ghost_rightWeakPoint)->damageable = true;
			be->setInternal(29, 0);
		}
	}
	boss2_display(be);
}

#undef attached

/*
 * ###################
 * Behaviour callbacks
 * ###################
 */

void boss1_cb(BossEnemy *be)
{
	Rect pos, centerRot;
	
	pos.x = fixtoi(be->getx());
	pos.y = fixtoi(be->gety());
	DC->add(bossImage_entries[bossImage_LUT_1_body], &pos, NULL, be->angle, be->currentPattern == 2 && be->flash, CAMREL_RELATIVE);
	
	be->currentPattern = getPatternID(be);
	if(be->currentPattern != be->lastPattern)
	{
		be->lastPattern = be->currentPattern;
		be->initCallbackCalled = false;
		be->readyToGo = false;
		Level::enemiesArray->destroyAllEnemies();
		Level::bArray->clear();
		return;
	}
	
	// [0] : timer
	
	// Pattern 1
	if(!be->currentPattern)
	{
		// Slowly move and rotate the boss to fire everywhere
		be->subx(fixcos(be->getInternal(0) / 2) / 2);
		be->angle = fixmul(12, fixsin(be->getInternal(0) / 2));
		be->incInternal(0);
		
		pos = getJointPoint(be, boss1_jointData, boss1_joint_rightarm_armed1);
		rotate(pos.x, pos.y, fixtoi(be->getx()), fixtoi(be->gety()), be->angle, &pos);
		centerRot.x = pos.w;
		centerRot.y = pos.h;
		DC->add(bossImage_entries[bossImage_LUT_1_rightarm_armed1], &pos, &centerRot, be->angle, false, CAMREL_RELATIVE);
		
		pos = getJointPoint(be, boss1_jointData, boss1_joint_leftarm_armed);
		rotate(pos.x, pos.y, fixtoi(be->getx()), fixtoi(be->gety()), be->angle, &pos);
		centerRot.x = pos.w;
		centerRot.y = pos.h;
		DC->add(bossImage_entries[bossImage_LUT_1_leftarm_armed], &pos, &centerRot, be->angle, be->flash, CAMREL_RELATIVE);
		
		int timer = be->getInternal(0);
		
		if(!(timer % 8))
		{
			rotate(pos.x, pos.y + bossImage_entries[bossImage_LUT_1_leftarm_armed][1], pos.x, pos.y, be->angle, &pos);
			if(timer & 8)
			{
				for(int i = 0; i < 3; i++)
				{
					Fixed angle = 64 - be->angle - (i - 1) * 32;
					Level::bArray->add(itofix(pos.x), itofix(pos.y), angle, itofix(2), image_LUT_enemy_bullet_1_light, LIGHT, true, CAMREL_NONE);
				}
			}
			else
			{
				for(int i = 0; i < 2; i++)
				{
					Fixed angle = 64 - be->angle - (i * 32 - 16);
					Level::bArray->add(itofix(pos.x), itofix(pos.y), angle, itofix(2), image_LUT_enemy_bullet_1_light, LIGHT, true, CAMREL_NONE);
				}
			}
		}
	}
	// Pattern 2
	else if(be->currentPattern == 1)
	{
		unsigned short *img = bossImage_entries[bossImage_LUT_1_rightarm_armed2];
		// Display things the exact same way as in pattern 1
		be->setx(be->getx() - fixcos(be->getInternal(0) / 2) / 2);
		be->angle = fixmul(12, fixsin(be->getInternal(0) / 2));
		be->incInternal(0);
		
		pos = getJointPoint(be, boss1_jointData, boss1_joint_leftarm_nonarmed);
		rotate(pos.x, pos.y, fixtoi(be->getx()), fixtoi(be->gety()), be->angle, &pos);
		centerRot.x = pos.w;
		centerRot.y = pos.h;
		DC->add(bossImage_entries[bossImage_LUT_1_leftarm_nonarmed], &pos, &centerRot, be->angle, false, CAMREL_RELATIVE);
		
		pos = getJointPoint(be, boss1_jointData, boss1_joint_rightarm_armed2);
		rotate(pos.x, pos.y, fixtoi(be->getx()), fixtoi(be->gety()), be->angle, &pos);
		centerRot.x = pos.w;
		centerRot.y = pos.h;
		DC->add(bossImage_entries[bossImage_LUT_1_rightarm_armed2], &pos, &centerRot, be->angle, be->flash, CAMREL_RELATIVE);
		
		int timer = be->getInternal(0);
		
		if(!(timer % 6))
		{
			static int bulletFired[3] = { 2, 2, 0 };
			static bool canFire[3] = { false, true, true };
			rotate(pos.x + img[0] / 2 - pos.w, pos.y + img[1] / 2 - pos.h , pos.x, pos.y, be->angle, &pos);
			for(int j = 0; j < 3; j++)
			{
				if(canFire[j])
				{
					Fixed angle = j * 6 + 16 - be->angle;
					for(int i = 0; i < 4; i++)
					{
						Level::bArray->add(itofix(pos.x), itofix(pos.y), angle, itofix(2), image_LUT_enemy_bullet_1_light, SHADOW, true, CAMREL_NONE);
						angle += 21;
					}
					bulletFired[j]++;
				}
				else
				{
					bulletFired[j]--;
				}
				if(!bulletFired[j] || bulletFired[j] == 4)
					canFire[j] = !canFire[j];
			}
		}
		
	}
	// Pattern 3
	else if(be->currentPattern == 2)
	{
		// [2] : left arm angle
		// [3] : right arm angle
		pos = getJointPoint(be, boss1_jointData, boss1_joint_leftarm_nonarmed);
		centerRot.x = pos.w;
		centerRot.y = pos.h;
		DC->add(bossImage_entries[bossImage_LUT_1_leftarm_nonarmed], &pos, &centerRot, be->getInternal(2), false, CAMREL_RELATIVE);
		
		pos = getJointPoint(be, boss1_jointData, boss1_joint_rightarm_nonarmed);
		rotate(pos.x, pos.y, fixtoi(be->getx()), fixtoi(be->gety()), be->angle, &pos);
		centerRot.x = pos.w;
		centerRot.y = pos.h;
		DC->add(bossImage_entries[bossImage_LUT_1_rightarm_nonarmed], &pos, &centerRot, be->getInternal(3), false, CAMREL_RELATIVE);
		
		be->incInternal(0);
		
		int timer = be->getInternal(0);
		
		if(!(timer % 128))
		{
			// [1] : fire stage
			// 0 - 5 : grenades
			// 6 : wait until the bullets from the grenades disappear
			// 7 - 10 : homings
			int fireStage = be->getInternal(1) % 11;
			if(fireStage < 6)
			{
				pos = getJointPoint(be, boss1_jointData, boss1_joint_leftarm_nonarmed);
				for(int i = 0; i < 4; i++)
					Level::enemiesArray->add(itofix(pos.x), itofix(pos.y), 1, image_LUT_boss1_grenade_light + (fireStage % 2), Pattern_1_bossGrenade, i, fireStage % 2, true, 0, true, false);
			}
			else if(fireStage > 6)
			{
				pos = getJointPoint(be, boss1_jointData, boss1_joint_rightarm_nonarmed);
				for(int i = 0; i < 5; i++)
				{
					Level::bArray->add_homing(itofix(pos.x), itofix(pos.y), i * 32, Level::p, LIGHT);
				}
			}
			be->incInternal(1);
		}
	}
	be->flash = false;
}

#define attached(x) be->attachedEnemies[x]
#define MOVE_DELAY 1

void boss2_cb(BossEnemy *be)
{
	Enemy *lwp = attached(boss2_ghost_leftWeakPoint), *rwp = attached(boss2_ghost_rightWeakPoint);
	if (!lwp->isActive())
		attached(boss2_ghost_leftShield)->internal[0] = 2;
	if (!rwp->isActive())
		attached(boss2_ghost_rightShield)->internal[0] = 2;
	be->HP = max(lwp->HP, 0) + max(rwp->HP, 0);
	
	// [0] : left wing angle
	// [1] : right wing angle
	// [28] : wings image index
	// [29] : sine movement phase
	// [30] : pattern timer
	// [31] : pattern stage indicator
	
	be->setx(itofix(160) + fixsin(be->getInternal(29) / 4) * (160 - bossImage_entries[be->body][0]));
	be->incInternal(29);

	boss2_display(be);

	// The weak points are protected by the shields when they are being covered by them
	Enemy *s = attached(boss2_ghost_leftShield), *w = attached(boss2_ghost_leftWeakPoint);
	w->damageable = !collidePointRect(w->getx(), w->gety(), s->getx(), s->gety(), s->img[0], s->img[1]);
	s = attached(boss2_ghost_rightShield); w = attached(boss2_ghost_rightWeakPoint);
	w->damageable = !collidePointRect(w->getx(), w->gety(), s->getx(), s->gety(), s->img[0], s->img[1]);
	
	if (!(G_gpTimer % MOVE_DELAY))
		be->incInternal(30);

	// Continuously fire bullets from the two top points (21, 54) and (118, 55)
	Fixed ox = itofix(bossImage_entries[be->body][0] / 2 - 21), oy = itofix(-bossImage_entries[be->body][1] / 2 + 54);
	if (!(G_gpTimer % 20))
		Level::bArray->add(be->getx() - ox, be->gety() + oy, 128 + Level::p->angleToXY(be->getx() - ox, be->gety() + oy) + ((rand() % 32) - 16), 192, image_LUT_enemy_bullet_0_light, LIGHT, true, be->getCamRel());
	else if (!((G_gpTimer + 10) % 20))
		Level::bArray->add(be->getx() + ox, be->gety() + oy, 128 + Level::p->angleToXY(be->getx() + ox, be->gety() + oy) + ((rand() % 32) - 16), 192, image_LUT_enemy_bullet_0_light, SHADOW, true, be->getCamRel());

	/* Pattern handling
	 * 0 : wait for 512 frames
	 * 1 : get the arms down
	 * 2 : shoot bullets over a quadrant of the arms 4 times
	 * 3 : wait for 128 frames
	 * 4 : shoot 4 waves of bullets
	 * 5 : get the arms up
	 * 6 : wait for 256 frames
	 * 7 : get the wings down + animation
	 * 8 : wait for 128 frames
	 * 9 : shoot 4 waves of bullets
	 * 10 : wait for 128 frames
	 * 11 : shoot a single wave of dual bullets
	 * 12 : animation + get the wings up and repeat
	 */

	// [2] : bullet count (2) (4) (9) (11)
	// [3] : bullets angle (2)
	// [4] : repetition count (2) (4) (9)
	switch (be->getInternal(31))
	{
	case 0:
		if (be->waitFrames(512))
			be->incInternal(31);
		break;
	case 1:
		if (attached(boss2_ghost_leftUpperArm)->rotationAngle < ANGLE_LEFTUPPERARM_DOWN)
		{
			attached(boss2_ghost_leftUpperArm)->rotationAngle++;
			attached(boss2_ghost_rightUpperArm)->rotationAngle--;
		}
		else
		{
			be->setInternal(2, 0);
			be->setInternal(3, 0);
			be->setInternal(4, 0);
			be->incInternal(31);
		}
		break;
	case 2:
		if (be->getInternal(4) < 4)
		{
			if (be->getInternal(2) < 16)
			{
				if (be->waitFrames(2))
				{
					Fixed centerX = be->getx();
					Fixed offset = abs(attached(boss2_ghost_leftArm)->getx() - itofix(bossImage_entries[bossImage_LUT_2_leftArm][0] / 2) + itofix(30) - centerX);
					Fixed centerY = attached(boss2_ghost_leftArm)->gety() - itofix(bossImage_entries[bossImage_LUT_2_leftArm][1] / 2) + itofix(50);
					for (int i = 0; i < 4; i++)
					{
						Fixed angle = be->getInternal(3) + 64 * i;
						if (be->getInternal(4) & 1)
							Level::bArray->add(centerX + offset + fixcos(angle) * 15, centerY + fixsin(angle) * 15, angle + 64, 128 + be->getInternal(2) * 32, image_LUT_enemy_bullet_0_light, SHADOW, true, CAMREL_NONE);
						else
							Level::bArray->add(centerX - offset + fixcos(-angle) * 15, centerY + fixsin(-angle) * 15, -angle - 64, 128 + be->getInternal(2) * 32, image_LUT_enemy_bullet_0_light, LIGHT, true, CAMREL_NONE);
					}
					be->incInternal(2);
					be->setInternal(3, be->getInternal(3) + 4);
				}
			}
			else
			{
				if (be->waitFrames(16))
				{
					be->setInternal(2, 0);
					be->setInternal(3, 0);
					be->incInternal(4);
				}
			}
		}
		else
			be->incInternal(31);
		break;
	case 3:
		if (be->waitFrames(128))
		{
			be->setInternal(2, 0);
			be->setInternal(4, 0);
			be->incInternal(31);
		}
		break;
	case 4:
		if (be->getInternal(4) < 4)
		{
			if (be->getInternal(2) < 32)
			{
				if (be->waitFrames(4))
				{
					// Hardcoded values for the positions from where to shoot the bullets
					int positions[] = { 45, 37, 48, 31, 52, 26, 55, 19, 56, 12, 54, 7 };
					Rect dir;
					dir.x = itofix(boss2_jointData[boss2_joint_leftArm][0] - bossImage_entries[bossImage_LUT_2_leftArm][0] / 2);
					dir.y = itofix(boss2_jointData[boss2_joint_leftArm][1] - bossImage_entries[bossImage_LUT_2_leftArm][1] / 2);
					Fixed a;
					bool odd = be->getInternal(4) & 1;
					if (odd)
					{
						dir.x = -dir.x;
						dir.w = attached(boss2_ghost_rightArm)->getx();
						dir.h = attached(boss2_ghost_rightArm)->gety();
						a = attached(boss2_ghost_rightArm)->angleToXY(dir.y + dir.w, -dir.x + dir.h) - 16;
					}
					else
					{
						dir.w = attached(boss2_ghost_leftArm)->getx();
						dir.h = attached(boss2_ghost_leftArm)->gety();
						a = attached(boss2_ghost_leftArm)->angleToXY(-dir.y + dir.w, dir.x + dir.h) + 16;
					}
					dir.w -= itofix(bossImage_entries[bossImage_LUT_2_leftArm][0]) / 2;
					dir.h -= itofix(bossImage_entries[bossImage_LUT_2_leftArm][1]) / 2;
					for (int k = 0; k < 6; k++)
					{
						Level::bArray->add(dir.w + itofix(odd ? bossImage_entries[bossImage_LUT_2_rightArm][0] - positions[k*2] : positions[k*2]),
								dir.h + itofix(positions[k*2+1]), a + (rand() % 32) - 16,
								192 + (rand() % 192), image_LUT_enemy_bullet_0_light, be->getInternal(4) & 1, true, CAMREL_NONE);
					}
					be->incInternal(2);
				}
			}
			else
			{
				be->setInternal(2, 0);
				be->incInternal(4);
			}
		}
		else if (be->waitFrames(64))
			be->incInternal(31);
		break;
	case 5:
		if (attached(boss2_ghost_leftUpperArm)->rotationAngle > ANGLE_LEFTUPPERARM_UP)
		{
			attached(boss2_ghost_leftUpperArm)->rotationAngle--;
			attached(boss2_ghost_rightUpperArm)->rotationAngle++;
		}
		else
			be->incInternal(31);
		break;
	case 6:
		if (be->waitFrames(256))
			be->incInternal(31);
		break;
	case 7:
		if (be->getInternal(0) < ANGLE_LEFTWING_DOWN)
		{
			if (be->waitFrames(1))
			{
				be->incInternal(0);
				be->decInternal(1);
			}
		}
		else
			be->incInternal(31);
		break;
	case 8:
		if (be->getInternal(28) < 4 && !(G_gpTimer % 8)) be->incInternal(28);
		if (be->waitFrames(128))
		{
			be->setInternal(2, 0);
			be->setInternal(4, 0);
			be->incInternal(31);
		}
		break;
	case 9:
		if (be->getInternal(4) < 4)
		{
			bool oddWave = be->getInternal(4) & 1;
			if (!(G_gpTimer % 6))
			{
				if (be->getInternal(2) < 6)
				{
					// Hardcoded positions of the left wing's cannons on bossImage_2_leftWingOpening_4
					int positions[] = { 88, 55, 75, 55, 62, 54, 49, 53, 36, 52, 23, 51 };
					for (int i = 0; i < 6; i++)
					{
						Rect pos;
						int offsetX, offsetY;
						// If the number of the wave is odd, mirror the shots to the center of the boss
						if (oddWave)
						{
							pos = getJointPoint(be, boss2_jointData, boss2_joint_rightWing);
							offsetX = bossImage_entries[bossImage_LUT_2_rightWing][0] - positions[i * 2] - pos.w;
							offsetY = positions[i * 2 + 1] - pos.h;
							rotate(pos.x + offsetX, pos.y + offsetY, pos.x, pos.y, be->getInternal(1), &pos);
							Level::bArray->add(itofix(pos.x), itofix(pos.y), -be->getInternal(1) + 64, 384, image_LUT_enemy_bullet_0_light, SHADOW, true, CAMREL_NONE);
						}
						else
						{
							pos = getJointPoint(be, boss2_jointData, boss2_joint_leftWing);
							offsetX = positions[i * 2] - pos.w;
							offsetY = positions[i * 2 + 1] - pos.h;
							rotate(pos.x + offsetX, pos.y + offsetY, pos.x, pos.y, be->getInternal(0), &pos);
							Level::bArray->add(itofix(pos.x), itofix(pos.y), -be->getInternal(0) + 64, 384, image_LUT_enemy_bullet_0_light, LIGHT, true, CAMREL_NONE);
						}
					}
					be->incInternal(2);
				}
				else
				{
					if (be->waitFrames(32))
					{
						be->setInternal(2, 0);
						be->incInternal(4);
					}
				}
			}
		}
		else
			be->incInternal(31);
		break;
	case 10:
		if (be->waitFrames(128))
		{
			be->setInternal(2, 0);
			be->setInternal(4, 0);
			be->incInternal(31);
		}
		break;
	case 11:
		if (!(G_gpTimer % 6))
		{
			if (be->getInternal(2) < 8)
			{
				// Hardcoded positions of the left wing's cannons on bossImage_2_leftWingOpening_4
				int positions[] = { 88, 55, 75, 55, 62, 54, 49, 53, 36, 52, 23, 51 };
				for (int i = 0; i < 6; i++)
				{
					Rect pos = getJointPoint(be, boss2_jointData, boss2_joint_rightWing);
					int offsetX = bossImage_entries[bossImage_LUT_2_rightWing][0] - positions[i * 2] - pos.w;
					int offsetY = positions[i * 2 + 1] - pos.h;
					rotate(pos.x + offsetX, pos.y + offsetY, pos.x, pos.y, be->getInternal(1), &pos);
					Level::bArray->add(itofix(pos.x), itofix(pos.y), -be->getInternal(1) + 64, 384, image_LUT_enemy_bullet_0_light, SHADOW, true, CAMREL_NONE);
					pos = getJointPoint(be, boss2_jointData, boss2_joint_leftWing);
					offsetX = positions[i * 2] - pos.w;
					offsetY = positions[i * 2 + 1] - pos.h;
					rotate(pos.x + offsetX, pos.y + offsetY, pos.x, pos.y, be->getInternal(0), &pos);
					Level::bArray->add(itofix(pos.x), itofix(pos.y), -be->getInternal(0) + 64, 384, image_LUT_enemy_bullet_0_light, LIGHT, true, CAMREL_NONE);
				}
				be->incInternal(2);
			}
			else
			{
				if (be->getInternal(28) > 0 && !(G_gpTimer % 8)) be->decInternal(28);
				if (be->waitFrames(128))
				{
					be->setInternal(2, 0);
					be->setInternal(4, 0);
					be->incInternal(31);
				}
			}
		}
		break;
	case 12:
		if (be->getInternal(0) > ANGLE_LEFTWING_UP)
		{
			if (be->waitFrames(1))
			{
				be->decInternal(0);
				be->incInternal(1);
			}
		}
		else
			be->setInternal(31, 0);
		break;
	default:
		break;
	}
}

#undef attached

/*
 * ###################
 * Collision callbacks
 * ###################
 */

/*
 * Boss 1
 */

// Hitbox : sword
int boss1_ccb1(BossEnemy *be, Bullet *b, int amount)
{
	Rect jointPos = getJointPoint(be, boss1_jointData, boss1_joint_leftarm_armed);
	const unsigned short *img = bossImage_entries[bossImage_LUT_1_leftarm_armed];
	Rect box;
	
	getBoundingBox(jointPos.x - jointPos.w, jointPos.y - jointPos.h, img[0], img[1], jointPos.x, jointPos.y, be->angle, &box);
	
	if(box.x <= fixtoi(b->getx()) && box.x + box.w >= fixtoi(b->getx()) &&
		box.y <= fixtoi(b->gety()) && box.y + box.h >= fixtoi(b->gety()))
	{
		G_score += b->getPolarity() == SHADOW ? SCORE_HIT_OP : SCORE_HIT;
		return ((b->getPolarity() == SHADOW) + 1) * amount;
	}
	return 0;
}

// Hitbox : shield
int boss1_ccb2(BossEnemy *be, Bullet *b, int amount)
{
	Rect jointPos = getJointPoint(be, boss1_jointData, boss1_joint_rightarm_armed2);
	const unsigned short *img = bossImage_entries[bossImage_LUT_1_rightarm_armed2];
	Rect box;
	
	getBoundingBox(jointPos.x - jointPos.w, jointPos.y - jointPos.h, img[0], img[1], jointPos.x, jointPos.y, be->angle, &box);
	
	if(box.x <= fixtoi(b->getx()) && box.x + box.w >= fixtoi(b->getx()) &&
		box.y <= fixtoi(b->gety()) && box.y + box.h >= fixtoi(b->gety()))
	{
		G_score += b->getPolarity() == LIGHT ? SCORE_HIT_OP : SCORE_HIT;
		return ((b->getPolarity() == LIGHT) + 1) * amount;
	}
	return 0;
}

// Hitbox : body
int boss1_ccb3(BossEnemy *be, Bullet *b, int amount)
{
	const unsigned short *img = bossImage_entries[bossImage_LUT_1_body];
	
	if(be->getx() - itofix(img[0]) / 2 <= b->getx() && be->getx() + itofix(img[0]) / 2 >= b->getx() &&
		be->gety() - itofix(img[1]) / 2 <= b->gety() && be->gety() + itofix(img[1]) / 2 >= b->gety())
	{
		G_score += b->getPolarity() == SHADOW ? SCORE_HIT_OP : SCORE_HIT;
		return ((b->getPolarity() == SHADOW) + 1) * amount;
	}
	return 0;
}

/**********
 * Boss 2 *
 **********/

#define attached(x) be->attachedEnemies[x]

// Hitbox : shields (weak points are handled as normal enemies)
int boss2_ccb(BossEnemy *be, Bullet *b, int amount)
{
	Enemy *le = attached(boss2_ghost_leftShield), *re = attached(boss2_ghost_rightShield);
	if (le->collide(b->getx(), b->gety()))
	{
		if (b->getPolarity() == le->getPolarity()) le->addy(min(itofix(60) - le->rawy(), itofix(2)));
		return -1;
	}
	if (re->collide(b->getx(), b->gety()))
	{
		if (b->getPolarity() == re->getPolarity()) re->addy(min(itofix(60) - re->rawy(), itofix(2)));
		return -1;
	}
	return 0;
}

#undef attached

/*
 * ##########################
 * Player collision callbacks
 * ##########################
 */

/*
 * Boss 1 : # TODO #
 */

// Hitbox : sword + body
bool boss1_pccb1(BossEnemy *be)
{
	return false;
}

// Hitbox : shield + body
bool boss1_pccb2(BossEnemy *be)
{
	return false;
}

// Hitbox : body
bool boss1_pccb3(BossEnemy *be)
{
	return false;
}

/*
 * Boss 2
 */

// Hitbox : body
bool boss2_pccb(BossEnemy *be)
{
	return Level::p->distance2ToXY(be->getx(), be->gety() - itofix(107 - 50)) <= itofix(64 * 64); // y - height/2 + 50 (y coordinate of the center of the ball ; see boss image)
}

/*
 * ##################
 * Distance callbacks
 * ##################
 */

/*
 * Boss 1
 */

// Target point : center of sword
Fixed boss1_dcb1(BossEnemy *be, PowerFragment *pf)
{
	Rect pos = getJointPoint(be, boss1_jointData, boss1_joint_leftarm_armed);
	rotate(pos.x, pos.y, fixtoi(be->getx()), fixtoi(be->gety()), be->angle, &pos);
	rotate(pos.x, pos.y + bossImage_entries[bossImage_LUT_1_leftarm_armed][1] / 2, pos.x, pos.y, be->angle, &pos);
	return distance(pos.x, pos.y, fixtoi(pf->getx()), fixtoi(pf->gety()));
}

// Target point : center of shield
Fixed boss1_dcb2(BossEnemy *be, PowerFragment *pf)
{
	Rect pos = getJointPoint(be, boss1_jointData, boss1_joint_rightarm_armed2);
	rotate(pos.x, pos.y, fixtoi(be->getx()), fixtoi(be->gety()), be->angle, &pos);
	return distance(pos.x, pos.y, fixtoi(pf->getx()), fixtoi(pf->gety()));
}

// Target point : center of body
Fixed boss1_dcb3(BossEnemy *be, PowerFragment *pf)
{
	return distance(fixtoi(be->getx()), fixtoi(be->gety()), fixtoi(pf->getx()), fixtoi(pf->gety()));
}

/*
 * Boss 2
 */

// Target points : weak points, handled as normal enemies
Fixed boss2_dcb(BossEnemy *be, PowerFragment *pf)
{
	return 0x7fffffff;
}

/*
 * ###############
 * Angle callbacks
 * ###############
 *
 * Same target points as the distance callbacks
 */

/*
 * Boss 1
 */

Fixed boss1_acb1(BossEnemy *be, PowerFragment *pf)
{
	Entity temp;
	Rect pos = getJointPoint(be, boss1_jointData, boss1_joint_leftarm_armed);
	rotate(pos.x, pos.y, fixtoi(be->getx()), fixtoi(be->gety()), be->angle, &pos);
	rotate(pos.x, pos.y + bossImage_entries[bossImage_LUT_1_leftarm_armed][1] / 2, pos.x, pos.y, be->angle, &pos);
	temp.activate();
	temp.setx(itofix(pos.x));
	temp.sety(itofix(pos.y));
	return pf->angleToEntity(&temp);
}

Fixed boss1_acb2(BossEnemy *be, PowerFragment *pf)
{
	Entity temp;
	Rect pos = getJointPoint(be, boss1_jointData, boss1_joint_rightarm_armed2);
	rotate(pos.x, pos.y, fixtoi(be->getx()), fixtoi(be->gety()), be->angle, &pos);
	temp.activate();
	temp.setx(itofix(pos.x));
	temp.sety(itofix(pos.y));
	return pf->angleToEntity(&temp);
}

Fixed boss1_acb3(BossEnemy *be, PowerFragment *pf)
{
	return pf->angleToEntity(be);
}

/*
 * Boss 2
 */

Fixed boss2_acb(BossEnemy *be, PowerFragment *pf)
{
	return 0;
}

/* ####################
 * 
 * ADDITIONAL DATA
 *
 * ####################
 */

boss_icb bossICBdata[BOSS_NB][MAX_PATTERNS_PER_BOSS] = {
	{ boss1_icb1, boss1_icb2, boss1_icb2 },
	{ boss2_icb, NULL, NULL }
};

boss_cb bossCBdata[BOSS_NB] = { boss1_cb, boss2_cb };

boss_ccb bossCCBdata[BOSS_NB][MAX_PATTERNS_PER_BOSS] = {
	{ boss1_ccb1, boss1_ccb2, boss1_ccb3 },
	{ boss2_ccb, NULL, NULL }
};

boss_pccb bossPCCBdata[BOSS_NB][MAX_PATTERNS_PER_BOSS] = {
	{ boss1_pccb1, boss1_pccb2, boss1_pccb3 },
	{ boss2_pccb, NULL, NULL }
};

boss_dcb bossDCBdata[BOSS_NB][MAX_PATTERNS_PER_BOSS] = {
	{ boss1_dcb1, boss1_dcb2, boss1_dcb3 },
	{ boss2_dcb, NULL, NULL }
};

boss_acb bossACBdata[BOSS_NB][MAX_PATTERNS_PER_BOSS] = {
	{ boss1_acb1, boss1_acb2, boss1_acb3 },
	{ boss2_acb, NULL, NULL }
};