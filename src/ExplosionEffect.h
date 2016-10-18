#ifndef INC_C
#define INC_C

#include "common.h"

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

extern void initExplosionEngine();
extern void initExplosionEffect(int x, int y, int coef, Fixed g);
extern bool updateExplosionEffect();
extern void renderExplosionEffect();
extern void deinitExplosionEngine();

#endif