#pragma once

#include "types.h"
#include "helpers/Constants.hpp"

// Particles
// Because they are pretty
class Particles
{
public:
	Particles();
	void add(Fixed x, Fixed y, Fixed angle, Fixed r, bool polarity, int lifetime);
	void pulse(Fixed x, Fixed y, bool polarity);
	void handle();
private:
	Fixed x[Constants::MAX_PARTICLE];
	Fixed y[Constants::MAX_PARTICLE];
	Fixed dx[Constants::MAX_PARTICLE];
	Fixed dy[Constants::MAX_PARTICLE];
	int time[Constants::MAX_PARTICLE];
	int dt[Constants::MAX_PARTICLE];
	bool polarity[Constants::MAX_PARTICLE];
	int counter;
};
