#pragma once

#include "types.h"
#include "helpers/Constants.hpp"

// Background scrolling handler
class BackgroundScroller
{
public:
	// x is left-top corner, y is center (ugly but purpose-built)
	BackgroundScroller(const unsigned short* bg, Fixed _x, Fixed _y, Fixed sscale, Fixed dscale, LUTs::BgTravelingId bgHandleID);
	void draw() const;
	void update();
	Fixed x;
	Fixed y;
	Fixed dx;
	Fixed dy;
	const unsigned short* img;
	Fixed w;
	Fixed h;
	unsigned short colorKey;
	Fixed scrollScale;
	Fixed displayScale;
	background_traveling handle;
};
