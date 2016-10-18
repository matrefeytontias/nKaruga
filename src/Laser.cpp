#include "common.h"

Laser::Laser() : Bullet()
{
	active = false;
	hurtPlayer = true;
}

Laser::~Laser()
{
	
}

void Laser::activate(Enemy *e, bool _p, Fixed _a)
{
	origin = e;
	active = true;
	polarity = _p;
	amplitude = 0;
	angleOffset = _a;
	angle = -origin->getRotation() + angleOffset;
	x = fixtoi(origin->getx()) + fixtoi(fixcos(angle) * origin->img[0] / 2);
	y = fixtoi(origin->gety()) + fixtoi(fixsin(angle) * origin->img[1] / 2);
	G_particles->pulse(itofix(x), itofix(y), polarity);
}

void Laser::handle()
{
	angle = -origin->getRotation() + angleOffset;
	x = fixtoi(origin->getx()) + fixtoi(fixcos(angle) * origin->img[0] / 2);
	y = fixtoi(origin->gety()) + fixtoi(fixsin(angle) * origin->img[1] / 2);
	amplitude += (amplitude < 320) * LASER_SPEED;
}

inline unsigned short r5g6b5(uint8_t r, uint8_t g, uint8_t b)
{
	return ((r >> 3) << 11) | ((g >> 2) << 5) | (b >> 3);
}

void Laser::draw() 
{
	int j = (rand() % 4) + 1;
	for(int i = 0; i < j; i++)
	{
		Fixed a = angle + (rand() % 64) - 32;
		G_particles->add(itofix(x), itofix(y), a, itofix(1), polarity, 32);
	}
	
	// A laser has an up-to 8-pixels core radius with an up-to 12-pixels aura radius (ie a laser is up to 24 pixels thick)
	// I use n2DLib's drawLine algorithm (by pierrotdu18), but drawing rotated laser slices instead of pixels
	int x1 = x;
	int y1 = y;
	int x2 = fixtoi(fixcos(angle) * amplitude) + x1;
	int y2 = fixtoi(fixsin(angle) * amplitude) + y1;
	int dx = abs(x2 - x1);
	int dy = abs(y2 - y1);
	int sx = (x1 < x2) ? 1 : -1;
	int sy = (y1 < y2) ? 1 : -1;
	int err = dx - dy;
	// Slice drawing vars
	Fixed sdx = fixsin(-angle);
	Fixed sdy = fixcos(-angle);
	Fixed phase = -G_gpTimer * 48;
	
	unsigned short coreColor = polarity ? 0 : 0xffff;
	while (!(x1 == x2 && y1 == y2))
	{
		phase += 5; // a period is 50 pixels
		Fixed coreR = fixcos(phase) * 2 + itofix(6); // the core radius oscillates between 4 and 8 pixels
		Fixed auraR = fixsin(phase) * 3 + itofix(9); // the aura radius oscillates between 6 and 12 pixels
		
		// Draws a rotated laser "slice"
		Fixed sxp = itofix(x1) - sdx * 12;
		Fixed syp = itofix(y1) - sdy * 12;
		for(int i = 0; i < 25; i++)
		{
			if(abs(i - 12) <= fixtoi(coreR)) // Core pixel
			{
				setPixel(fixtoi(sxp), fixtoi(syp), coreColor);
				setPixel(fixtoi(sxp) + 1, fixtoi(syp), coreColor);
				setPixel(fixtoi(sxp), fixtoi(syp) + 1, coreColor);
				setPixel(fixtoi(sxp) + 1, fixtoi(syp) + 1, coreColor);
			}
			else if(abs(i - 12) <= fixtoi(auraR)) // Aura pixel
				setPixel(fixtoi(sxp), fixtoi(syp),
					polarity ?
						0xf800
						: r5g6b5(abs(fixtoi(auraR) - abs(i - 12)) * 256 / fixtoi(auraR), abs(fixtoi(auraR) - abs(i - 12)) * 128 / fixtoi(auraR) + 128, 255));
			sxp += sdx;
			syp += sdy;
		}
		
		int e2 = err * 2;
		if (e2 > -dy)
		{	
			err -= dy;
			x1 += + sx;
		}
		if (e2 < dx)
		{	
			err += dx;
			y1 += sy;
		}
	}
}

Rect* Laser::getVector()
{
	static Rect r;
	// (x, y) is origin
	// (w, h) are the vector's coordinates
	r.x = x;
	r.y = y;
	r.w = fixtoi(fixcos(angle) * amplitude);
	r.h = fixtoi(fixsin(angle) * amplitude);
	
	return &r;
}

void Laser::getSides(Rect* s1, Rect* s2)
{
	s1->x = x - fixtoi(fixsin(-angle) * LASER_THICKNESS / 2);
	s1->y = y - fixtoi(fixcos(-angle) * LASER_THICKNESS / 2);
	s2->x = x + fixtoi(fixsin(-angle) * LASER_THICKNESS / 2);
	s2->y = y + fixtoi(fixcos(-angle) * LASER_THICKNESS / 2);
}

int Laser::getAmplitude()
{
	return amplitude;
}

void Laser::setAmplitude(int _a)
{
	amplitude = _a;
}
