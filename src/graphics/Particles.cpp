#include "graphics/Particles.hpp"

#include "GameSystems.hpp"
#include "helpers/Constants.hpp"
#include "n2DLib/n2DLib.h"
#include "n2DLib/n2DLib_math.h"

Particles::Particles()
{
	for(int i = 0; i < Constants::MAX_PARTICLE; i++)
		time[i] = 0;
	counter = 0;
}

void Particles::add(Fixed _x, Fixed _y, Fixed _a, Fixed _r, bool _p, int lifetime)
{
	int ratio = (rand() % 256) - 128 + _r;
	x[counter] = _x;
	y[counter] = _y;
	dx[counter] = fixmul(fixcos(_a), ratio);
	dy[counter] = fixmul(fixsin(_a), ratio);
	polarity[counter] = _p;
	time[counter] = lifetime;
	dt[counter] = lifetime / Constants::PARTICLE_RADIUS;
	counter = (counter + 1) % Constants::MAX_PARTICLE;
}

void Particles::pulse(Fixed _x, Fixed _y, bool _p)
{
	for (int i = 0; i < 256; i++)
	{
		add(_x, _y, i, itofix(3), _p, Constants::FPS);
	}
}

void Particles::handle()
{
	for(int i = 0; i < Constants::MAX_PARTICLE; i++)
	{
		if(time[i])
		{
			time[i]--;
			x[i] += dx[i];
			y[i] += dy[i];
			
			fillCircle(fixtoi(x[i]), fixtoi(y[i]), time[i] / dt[i], polarity[i] ? 0 : 0xffff);
		}
	}
}
