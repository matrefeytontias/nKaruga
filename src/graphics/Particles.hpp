#pragma once

#include "types.h"

// TODO : move in Constants.hpp
#define MAX_PARTICLE 1024
#define PARTICLE_RADIUS 4

// Particles
// Because they are pretty
class Particles
{
public:
	Particles();
	~Particles();
	void add(Fixed x, Fixed y, Fixed angle, Fixed r, bool polarity, int lifetime);
	void pulse(Fixed x, Fixed y, bool polarity);
	void handle();
private:
	Fixed x[MAX_PARTICLE];
	Fixed y[MAX_PARTICLE];
	Fixed dx[MAX_PARTICLE];
	Fixed dy[MAX_PARTICLE];
	int time[MAX_PARTICLE];
	int dt[MAX_PARTICLE];
	bool polarity[MAX_PARTICLE];
	int counter;
};
