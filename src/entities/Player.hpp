#pragma once

#include "entities/Entity.hpp"

// Player
class Player : public Entity
{
public:
	Player();
	void reset();
	void handle(KeyEvent k);
	bool getPolarity() const;
	void switchPolarity();
	void hurt();
	int getLives() const;
	void setLives(int);
	bool isHurtable() const;
	// Ship images, light and shadow
	// First two are "normal" ship
	// Other four are polarity transition animation frames (frame 0 light/shadow, frame 1 light/shadow)
	// images have same dimensions
	const unsigned short* img[6];
	int deathCounter;
private:
	int isSwitchingPolarity;
	// keep firing at a reasonable rate
	int fireDelay;
	// True is light, false is shadow
	bool polarity;
	// Key repeat handlers
	bool fireRepeat;
	bool polarityRepeat;
	int lives;
};
