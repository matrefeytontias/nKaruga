#pragma once

#include "entities/Entity.hpp"
#include "helpers/Constants.hpp"

class Joint;

// Enemy
// For real this time

class Enemy : public Entity
{
	friend Joint;
public:
	Enemy();
	~Enemy();
	void handle();
	void activate(int x, int y, int HP, LUTs::BaseImageId shipImgId, LUTs::EnemyPatternId patternId, int waveIndex, bool polarity, bool hasRotation, int firebackAmount, bool ghost, Constants::EnemyType type);
	bool damage(bool polarity, int amount);
	void joint(Entity* target, int targetX, int targetY, int jointX, int jointY, int jointCX, int jointCY, unsigned short* timg, unsigned short* jimg, bool diesWithJoint);
	void joint(Entity* target, int targetX, int targetY, int targetCX, int targetCY, int jointX, int jointY, int jointCX, int jointCY, unsigned short* timg, unsigned short* jimg, bool diesWithJoint);
	bool collide(Fixed x, Fixed y, Fixed cx = 0, Fixed cy = 0);
	Fixed getRotation();
	void setRotation(Fixed angle);
	bool getPolarity();
	int getWaveIndex();
	bool isGhost();
	bool isProp();
	bool isDamageable();
	Fixed rawx();
	Fixed rawy();
	Fixed getx();
	Fixed gety();
	// x, y on-screen
	// Enemy image
	bool visible;
	unsigned short* img;
	bool diedThisFrame;
	//
	// Used by patterns
	//
	Fixed rotationAngle;
	// What bullets does the enemy fire
	bool polarity;
	// The position of the enemy in the wave
	int waveIndex;
	// Internal data for free use by the behaviour code
	int internal[6];
	// Arrays to use with n2DLib's interpolatePathFloat
	// TODO : get rid of that shit
	float* ax, * ay;
	int* at;
	void setAX(int, ...);
	void setAY(int, ...);
	void setAT(int, ...);
	bool damageable;
	int HP, maxHP;
	// Box routine
	void beAbox();
	// Prop routine
	void beAprop();

	// Used to constraint an enemy to another
	Joint* jointObj;
	bool isJointed;
private:
	// Keep track of the image ID for animation purposes.
	LUTs::BaseImageId shipImgId;
	// Ghost enemies have no interaction with anything
	bool ghost;
	// Props follow the absolute camera
	bool prop;
	// Does the enemy use rotation (achieved in the pattern)
	bool hasRotation;
	// The enemy's behaviour
	//int callback;
	enemy_pattern callback;

	//bool isJointed;
	bool diesWithJoint;
	//Entity *jointedTo;
	//Fixed jointX;
	//Fixed jointY;
	// How many bullets does the enemy fireback when dying
	int fireback;
	// Used for complex spawning schemes
	// Is 0 when the enemy can be deactivated by leaving the screen
	int spawned;
	// If the enemy has just been hit and its sprite needs to flash
	bool flash;
};