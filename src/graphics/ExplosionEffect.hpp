#pragma once

#include "types.h"

class ExpParticle
{
public:
	ExpParticle();
	~ExpParticle();
	void activate(Fixed x, Fixed y, Fixed dx, Fixed dy, Fixed gravity);
	void deactivate();
	bool isActive();
	void update();
	Fixed x, y, dx, dy, gravity;
private:
	unsigned char cIndex;
	bool active;
};

void initExplosionEngine();
void initExplosionEffect(int x, int y, int coef, Fixed g);
bool updateExplosionEffect();
void renderExplosionEffect();
void deinitExplosionEngine();
