#include "graphics/BackgroundScroller.hpp"

#include "n2DLib/n2DLib.h"
#include "n2DLib/n2DLib_math.h"

// TODO : LUTs::BgTravelingId bgTravelingId
BackgroundScroller::BackgroundScroller(const unsigned short *bg, Fixed _x, Fixed _y, Fixed sscale, Fixed dscale, int bgTravelingId) // x is left-top corner, y is center (ugly but purpose-built)
{
	img = &(bg[3]);
	w = bg[0];
	h = bg[1];
	colorKey = bg[2];
	x = itofix(_x);
	y = itofix(_y - h / 2);
	dx = 0;
	dy = 0;
	scrollScale = sscale;
	displayScale = dscale;
	handle = LUTs::bgTraveling(static_cast<LUTs::BgTravelingId>(bgTravelingId));
}

void BackgroundScroller::draw() const
{
	Fixed sourceX = -((x + fixmul(dx, scrollScale)) % itofix(w)) + itofix(w),
		sourceY = -((y + fixmul(dy, scrollScale)) % itofix(h)) + itofix(h);
	sourceX %= itofix(w);
	sourceY %= itofix(h);
	Fixed originX = sourceX;
	
	for(int sy = 0; sy < 240; sy++, sourceY = (sourceY + displayScale) % itofix(h))
	{
		sourceX = originX;
		for(int sx = 0, sourceX = originX; sx < 320; sx++, sourceX = (sourceX + displayScale) % itofix(w))
		{
			unsigned short c = img[fixtoi(sourceY) * w + fixtoi(sourceX)];
			if (c != colorKey)
				setPixelUnsafe(sx, sy, c);
		}
	}
}

void BackgroundScroller::update()
{
	(handle)(this);
}
