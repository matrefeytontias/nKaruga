#include "entities/Enemy.hpp"

#include <cstdlib>

#include "helpers/math.hpp"
#include "patterns.h"
#include "types.h"
#include "handlers/DrawingCandidates.hpp"
#include "helpers/Joint.hpp"

Enemy::Enemy() : Entity()
{
	active = false;
	diedThisFrame = false;
	isJointed = false;
	rotationAngle = 0;
	ax = NULL;
	ay = NULL;
	at = NULL;
	prop = false;
	damageable = false;
	visible = true;
	shipImgId = static_cast<int>(LUTs::BaseImageId::NONE);
	callback = NULL;
	spawned = 0;
	flash = false;
	isEnemy = true;
	jointObj = new Joint();
}

Enemy::~Enemy()
{
	if (ax) free(ax);
	if (ay) free(ay);
	if (at) free(at);
	delete jointObj;
}

void Enemy::handle()
{
	Rect er;
	Rect screenRect;
	
	if(active)
	{
		if(isJointed && diesWithJoint && !jointObj->target->isActive())
			damage(!polarity, HP);
		else
		{
			// handle bullets and pattern first
			(callback)(this);
			
			// Bring everything back to screen coordinates for collisions
			// Normal enemies have coordinates relative to the starting point of the wave
			// Props have absolute coordinates (relative to the starting point of the chapter)
			
			er.x = fixtoi(getx());
			er.y = fixtoi(gety());
			screenRect.x = fToScreenX(getx(), getCamRel());
			screenRect.y = fToScreenY(gety(), getCamRel());
			
			spawned -= !!spawned;
			// Have a relatively big threshold for off-screen movement
			// Props and ghosts are not deleted if they are outside the screen ; they are supposed to delete themselves when needed
			if (!prop && !ghost && (fixtoi(screenRect.x) < -img[0] * 2 || fixtoi(screenRect.x) > 319 + img[0] ||
				fixtoi(screenRect.y) < -img[1] * 2 || fixtoi(screenRect.y) > 239 + img[1]))
			{
				if (!spawned) deactivate();
			}
			else if (visible)
			{
				// then the enemy image
				if(hasRotation)
				{
					DC->add(img, &er, NULL, rotationAngle, flash, getCamRel());
				}
				else
				{
					DC->add(img, &er, flash, getCamRel());
				}
				flash = false;
			}
			if (!ghost)
			{
				// Check wether the player hit the enemy
				if(Level::p->isHurtable() && (Level::p->getx() >= screenRect.x - (itofix(img[0]) / 2) && Level::p->getx() < screenRect.x + (itofix(img[0]) / 2)) && (Level::p->gety() >= screenRect.y - (itofix(img[1]) / 2) && Level::p->gety() <= screenRect.y + (itofix(img[1]) / 2)))
				{
					Level::p->hurt();
				}
			}
		}
	}
	else
	{
		diedThisFrame = false;
		isJointed = false;
	}
}

#define SPAWN_DELAY 512

// TODO : LUTs::BaseImageId _shipImgId
void Enemy::activate(int _x, int _y, int _HP, int _shipImgId, int callbackID, int _waveIndex, bool _polarity, bool _hasRotation, int _f, bool _ghost, int type)
{
	maxHP = HP = _HP;
	x = _x;
	y = _y;
	
	shipImgId = _shipImgId;
	img = LUTs::baseImage(static_cast<LUTs::BaseImageId>(shipImgId));
	fireback = _f;
	polarity = _polarity;
	hasRotation = _hasRotation;
	rotationAngle = 0;
	callback = enemyCallbacks[callbackID];
	waveIndex = _waveIndex;
	for(int i = 0; i < 6; i++)
		internal[i] = 0;
	ghost = _ghost;
	prop = type != TYPE_ENEMY;
	damageable = type != TYPE_PROP && !ghost;
	active = true;
	camRel = prop ? CAMREL_ABSOLUTE : CAMREL_RELATIVE;
	diedThisFrame = false;
	spawned = SPAWN_DELAY;
	flash = false;
	isJointed = false;
}

// Returns whether or not the enemy was damaged
bool Enemy::damage(bool _pol, int amount)
{
	if (damageable)
	{
		flash = true;
		HP -= amount;
		if (_pol != polarity)
			HP -= amount;

		if (HP <= 0)
		{
			Level::soundSystem->quickPlaySFX(LUTs::sound(maxHP > 20 ? LUTs::SoundId::ENEMY_EXP_BIG : LUTs::SoundId::ENEMY_EXP_SMALL));
			if (G_fireback)
			{
				if (_pol == polarity || G_hardMode)
				{
					Fixed angle = angleToEntity(Level::p);
					int famount = _pol != polarity ? fireback / 2 : fireback;
					for (int i = 0; i < famount; i++)
						Level::bArray->add(getx(), gety(), angle + (rand() % 16) - 8, itofix(4) + (rand() % 512) - 256, static_cast<int>(LUTs::BaseImageId::ENEMY_BULLET_0_LIGHT), polarity, true, getCamRel());
				}
			}
			diedThisFrame = true;
			deactivate();
		}
	}
	return damageable;
}

void Enemy::joint(Entity *target, int targetX, int targetY, int jointX, int jointY, int jointCX, int jointCY, unsigned short *timg, unsigned short *jimg, bool _d)
{
	isJointed = true;
	jointObj->activate(target, targetX, targetY, jointX, jointY, jointCX, jointCY, timg, jimg);
	diesWithJoint = _d;
}

void Enemy::joint(Entity *target, int targetX, int targetY, int targetCX, int targetCY, int jointX, int jointY, int jointCX, int jointCY, unsigned short *timg, unsigned short *jimg, bool _d)
{
	isJointed = true;
	jointObj->activate(target, targetX, targetY, targetCX, targetCY, jointX, jointY, jointCX, jointCY, timg, jimg);
	diesWithJoint = _d;
}

bool Enemy::collide(Fixed _x, Fixed _y, Fixed _cx, Fixed _cy)
{
	Fixed x = fToScreenX(getx(), getCamRel()), y = fToScreenY(gety(), getCamRel());
	Fixed w = itofix(img[0]), h = itofix(img[1]);
	Rect temp;
	rotate(_x, _y, x + _cx, y + _cy, hasRotation ? -rotationAngle : 0, &temp);
	return temp.x < x + w / 2 && temp.x > x - w / 2 && temp.y < y + h / 2 && temp.y > y - h / 2;
}

Fixed Enemy::getRotation()
{
	return rotationAngle;
}

void Enemy::setRotation(Fixed a)
{
	rotationAngle = a;
}

bool Enemy::getPolarity()
{
	return polarity;
}

int Enemy::getWaveIndex()
{
	return waveIndex;
}

bool Enemy::isGhost()
{
	return ghost;
}

bool Enemy::isDamageable()
{
	return damageable;
}

bool Enemy::isProp()
{
	return prop;
}

Fixed Enemy::rawx()
{
	return x;
}

Fixed Enemy::rawy()
{
	return y;
}

Fixed Enemy::getx()
{
	return isJointed ? rawx() + jointObj->getx() : rawx();
}

Fixed Enemy::gety()
{
	return isJointed ? rawy() + jointObj->gety() : rawy();
}

void Enemy::setAX(int nb, ...)
{
	va_list args;
	if(ax) free(ax);
	ax = (float*)malloc(nb * sizeof(float));
	va_start(args, nb);
	for (int i = 0; i < nb; i++)
	{
		// va_arg does not take float
		// https://stackoverflow.com/questions/11270588/variadic-function-va-arg-doesnt-work-with-float
		float f = static_cast<float>(va_arg(args, double));
		ax[i] = f;
	}
	va_end(args);
}

void Enemy::setAY(int nb, ...)
{
	va_list args;
	if (ay) free(ay);
	ay = (float*)malloc(nb * sizeof(float));
	va_start(args, nb);
	for (int i = 0; i < nb; i++)
	{
		float f = static_cast<float>(va_arg(args, double));
		ay[i] = f;
	}
	va_end(args);
}

void Enemy::setAT(int nb, ...)
{
	va_list args;
	if (at) free(at);
	at = (int*)malloc(nb * sizeof(int));
	va_start(args, nb);
	for (int i = 0; i < nb; i++)
		at[i] = va_arg(args, int);
	va_end(args);
}

// Automagic box handling routine
// internal[0] and internal[1] define what the box does
// internal[4] and internal[5] handle animation internally, don't use them
// internal[0] : BITFIELD for shooting (not values) : 0 ? down, 1 ? left, 2 ? right, 3 ? up
// internal[2] : BITFIELD for number of bullets : 0 to shoot 3 bullets, 1 to shoot 1 bullet (same directions as internal[0])
// internal[1] : VALUES for moving : 1 ? down, 2 ? left, 3 ? right, 4 ? up ; anything else means no movement
void Enemy::beAbox()
{
	Fixed dx = 0, dy = 0;
	// Moving boxes
	switch (internal[1] - 1)
	{
	case 0:
		dy = 256 - FCAMERA_SPEED;
		break;
	case 1:
		dx = -256;
		break;
	case 2:
		dx = 256;
		break;
	case 3:
		dy = -256;
	}

	x += dx;
	y += dy;

	// Shooting boxes
	if (!(G_gpTimer % 16))
	{
		Fixed r = itofix(1);
		if (internal[0] & 1)
		{
			Fixed a = (Fixed)(atan2((float)dy + r, (float)dx) * 128 / M_PI);
			if (!(internal[2] & 1))
				for (int i = 0; i < 3; i++)
					Level::bArray->add(x + itofix(i - 1) * 10, y + itofix(24), a, r, static_cast<int>(LUTs::BaseImageId::ENEMY_BULLET_1_LIGHT), polarity, true, getCamRel());
			else
				Level::bArray->add(x, y + itofix(24), a, 192, static_cast<int>(LUTs::BaseImageId::ENEMY_BULLET_1_LIGHT), polarity, true, getCamRel());
		}
		if (internal[0] & 2)
		{
			Fixed a = 128 - (Fixed)(asin((float)dy / (float)r) * 128 / M_PI);
			if (!(internal[2] & 2))
				for (int i = 0; i < 3; i++)
					Level::bArray->add(x - itofix(24), y + itofix(i - 1) * 10, a, r, static_cast<int>(LUTs::BaseImageId::ENEMY_BULLET_1_LIGHT), polarity, true, getCamRel());
			else
				Level::bArray->add(x - itofix(24), y, a, r, static_cast<int>(LUTs::BaseImageId::ENEMY_BULLET_1_LIGHT), polarity, true, getCamRel());
		}
		if (internal[0] & 4)
		{
			Fixed a = (Fixed)(asin((float)dy / (float)r) * 128 / M_PI);

			if (!(internal[2] & 4))
				for (int i = 0; i < 3; i++)
					Level::bArray->add(x + itofix(24), y + itofix(i - 1) * 10, a, r, static_cast<int>(LUTs::BaseImageId::ENEMY_BULLET_1_LIGHT), polarity, true, getCamRel());
			else
				Level::bArray->add(x + itofix(24), y, a, r, static_cast<int>(LUTs::BaseImageId::ENEMY_BULLET_1_LIGHT), polarity, true, getCamRel());
		}
		if (internal[0] & 8)
		{
			Fixed a = (Fixed)(atan2((float)dy - r, (float)dx) * 128 / M_PI);
			if (!(internal[2] & 8))
				for (int i = 0; i < 3; i++)
					Level::bArray->add(x + itofix(i - 1) * 10, y - itofix(24), a, r, static_cast<int>(LUTs::BaseImageId::ENEMY_BULLET_1_LIGHT), polarity, true, getCamRel());
			else
				Level::bArray->add(x, y - itofix(24), a, r, static_cast<int>(LUTs::BaseImageId::ENEMY_BULLET_1_LIGHT), polarity, true, getCamRel());
		}
	}
	// Animation
	if (!internal[5])
	{
		internal[4] = rand() % 6;
		internal[5] = 1;
	}

	if (!(G_gpTimer % 3))
	{
		internal[4]++;
		internal[4] %= 6;
		img = LUTs::baseImage(static_cast<LUTs::BaseImageId>(shipImgId), internal[4]);
	}
	// a box is also a prop
	if (fToScreenY(y, getCamRel()) > itofix(300))
		deactivate();
}

void Enemy::beAprop()
{
	if (fToScreenY(y, getCamRel()) > itofix(300) || fToScreenX(x, getCamRel()) < itofix(-100) || fToScreenX(x, getCamRel()) > itofix(400))
		deactivate();
}
