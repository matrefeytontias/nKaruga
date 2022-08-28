#pragma once

#include "types.h"

class Entity;

class Joint
{
public:
	Joint();
	~Joint();
	void activate(Entity* target, int targetX, int targetY, int jointX, int jointY, int jointCX, int jointCY, unsigned short* timg, unsigned short* jimg);
	void activate(Entity* target, int targetX, int targetY, int targetCX, int targetCY, int jointX, int jointY, int jointCX, int jointCY, unsigned short* timg, unsigned short* jimg);
	Fixed getx();
	Fixed gety();
	Entity* target;
	unsigned short* timg; // the target's image
	unsigned short* jimg; // the jointed image
	int targetX, targetY; // the connection point on the target
	int targetCX, targetCY; // the target's rotation center if relevant
	int jointX, jointY; // the connection point on the jointed image
	int jointCX, jointCY; // the jointed image's rotation center ; pass -1 to default to the center
};
