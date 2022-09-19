#include "graphics/ExplosionEffect.hpp"

#include <cstdlib>

#include "n2DLib/n2DLib.h"
#include "n2DLib/n2DLib_math.h"

typedef struct
{
	unsigned char r, g, b;
} colorRGB;

unsigned char explosionBuffer[320][240];
#define MAX_EXPLOSION_PARTICLE 500
ExpParticle *explosionArray[MAX_EXPLOSION_PARTICLE];
colorRGB palette[256];
Rect areaToCheck;
bool explosionGoing = false;

//
// ExpParticle class
//

ExpParticle::ExpParticle()
{

}

void ExpParticle::activate(Fixed _x, Fixed _y, Fixed _dx, Fixed _dy, Fixed _g)
{
	x = itofix(_x);
	y = itofix(_y);
	dx = _dx;
	dy = _dy;
	gravity = _g;
	cIndex = 254;
	active = true;
}

void ExpParticle::deactivate()
{
	active = false;
}

bool ExpParticle::isActive() const
{
	return active;
}

void ExpParticle::update()
{
	if(fixtoi(x) > 0 && fixtoi(x) < 319 && fixtoi(y) > 0 && fixtoi(y) < 239)
	{
		explosionBuffer[fixtoi(x)][fixtoi(y)] = cIndex;
		explosionBuffer[fixtoi(x) - 1][fixtoi(y)] = cIndex;
		explosionBuffer[fixtoi(x) + 1][fixtoi(y)] = cIndex;
		explosionBuffer[fixtoi(x)][fixtoi(y) - 1] = cIndex;
		explosionBuffer[fixtoi(x)][fixtoi(y) + 1] = cIndex;

		if(cIndex != 0)
			cIndex -= 2;
		else
			deactivate();
		x += dx;
		y += dy;
		dy += gravity;
	}
	else
		active = false;
}

//
// Explosion rendering engine
//

void initExplosionEngine()
{
	for(int i = 0; i < MAX_EXPLOSION_PARTICLE; i++)
	{
		explosionArray[i] = new ExpParticle();
		explosionArray[i]->deactivate();
	}
	
	// Empty palette
	for(int i = 0; i < 256; i++)
	{
		palette[i].r = 0;
		palette[i].g = 0;
		palette[i].b = 0;
	}

	// Create a suitable fire palette, this is crucial for a good effect
	// Black to blue, blue to red, red to yellow, yellow to white

	// for(int i = 0; i < 32; ++i)
	for (int i = 0; i < 16; ++i)
	{
		// Black to blue, 16 values
		palette[i].b = i << 4;
		// Blue to red, 16 values
		palette[i + 16].r = i << 3;
		palette[i + 16].b = 64 - (i << 2);
		// Red to yellow, 32 values
		palette[i + 32].r = 255;
		palette[i + 32].g = i << 4;
		palette[i + 48].r = 255;
		palette[i + 48].g = (i + 16) << 3;
		// Yellow to white, 192 values
		palette[i + 64].r = 255;
		palette[i + 64].g = 255;
		palette[i + 64].b = i;
		palette[i + 80].r = 255;
		palette[i + 80].g = 255;
		palette[i + 80].b = i + 21;
		palette[i + 96].r = 255;
		palette[i + 96].g = 255;
		palette[i + 96].b = i + 42;
		palette[i + 112].r = 255;
		palette[i + 112].g = 255;
		palette[i + 112].b = i + 63;
		palette[i + 128].r = 255;
		palette[i + 128].g = 255;
		palette[i + 128].b = i + 84;
		palette[i + 144].r = 255;
		palette[i + 144].g = 255;
		palette[i + 144].b = i + 105;
		palette[i + 160].r = 255;
		palette[i + 160].g = 255;
		palette[i + 160].b = i + 126;
		palette[i + 176].r = 255;
		palette[i + 176].g = 255;
		palette[i + 176].b = i + 147;
		palette[i + 192].r = 255;
		palette[i + 192].g = 255;
		palette[i + 192].b = i + 168;
		palette[i + 208].r = 255;
		palette[i + 208].g = 255;
		palette[i + 208].b = i + 189;
		palette[i + 224].r = 255;
		palette[i + 224].g = 255;
		palette[i + 224].b = i + 210;
		palette[i + 240].r = 255;
		palette[i + 240].g = 255;
		palette[i + 240].b = i + 231;
	}
}

void initExplosionEffect(int x, int y, int coef, Fixed g)
{
	int am = clamp(coef, 0, MAX_EXPLOSION_PARTICLE);
	// Empty explosion buffer
	for(int j = 0; j < 240; j++)
		for(int i = 0; i < 320; i++)
			explosionBuffer[i][j] = 0;
	
	for(int i = 0; i < MAX_EXPLOSION_PARTICLE; i++)
		explosionArray[i]->deactivate();
	
	// Activate particles
	for(int i = 0; i < am; i++)
	{
		Fixed a = rand() % 256;
		Fixed r = fixmul((rand() % itofix(5)) - itofix(3), coef / 2);
		explosionArray[i]->activate(x, y, fixmul(fixcos(a), r), fixmul(fixsin(a), r), g);
	}
	explosionGoing = false;
}

// Returns whether or not the effect finished
bool updateExplosionEffect()
{
	bool effectDone = true;
	areaToCheck.x = 319;
	areaToCheck.y = 239;
	areaToCheck.w = 1;
	areaToCheck.h = 1;
	
	// Handle particles
	for(int i = 0; i < MAX_EXPLOSION_PARTICLE; i++)
	{
		if(explosionArray[i]->isActive())
		{
			explosionArray[i]->update();
			areaToCheck.x = max(1, min(areaToCheck.x, fixtoi(explosionArray[i]->x)));
			areaToCheck.y = max(1, min(areaToCheck.y, fixtoi(explosionArray[i]->y)));
			areaToCheck.w = min(319, max(areaToCheck.w, fixtoi(explosionArray[i]->x)));
			areaToCheck.h = min(239, max(areaToCheck.h, fixtoi(explosionArray[i]->y)));
		}
	}
	
	// Erase unwanted parts
	for(int i = 0; i < 320; i++)
		explosionBuffer[i][0] = explosionBuffer[i][239] = 0;
	for(int i = 0; i < 240; i++)
		explosionBuffer[0][i] = explosionBuffer[319][i] = 0;
	
	// Shift the fire buffer
	for(int y = areaToCheck.y; y < areaToCheck.h; y++)
	{
		for(int x = areaToCheck.x; x < areaToCheck.w; x++)
		{
			int temp;
			int x1 = x - 1, x2 = x + 1, y1 = y - 1, y2 = y + 1;

			temp = explosionBuffer[x1][y1];
			temp += explosionBuffer[x][y1];
			temp += explosionBuffer[x2][y1];
			temp += explosionBuffer[x1][y];
			temp += explosionBuffer[x2][y];
			temp += explosionBuffer[x1][y2];
			temp += explosionBuffer[x][y2];
			temp += explosionBuffer[x2][y2];
			temp /= 8;

			if(temp > 4)
			{
				temp -= 4;
				effectDone = false;
			}
			else
				temp = 0;

			explosionBuffer[x][y] = temp;
		}
	}
	
	explosionGoing = !effectDone;
	return effectDone;
}

void renderExplosionEffect()
{
	if(explosionGoing)
	{
		// Draw it
		for(int y = areaToCheck.y; y < areaToCheck.h; y++)
		{
			for(int x = areaToCheck.x; x < areaToCheck.w; x++)
			{
				if(explosionBuffer[x][y])
				{
					colorRGB curC = palette[explosionBuffer[x][y]];
					setPixel(x, y, rgbToPixel(curC.r, curC.g, curC.b));
				}
			}
		}
	}
}

void deinitExplosionEngine()
{
	for(int i = 0; i < MAX_EXPLOSION_PARTICLE; i++)
		delete explosionArray[i];
}