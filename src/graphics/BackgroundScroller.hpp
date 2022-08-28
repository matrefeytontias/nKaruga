#pragma once

#include "types.h"

class BackgroundScroller;

// Update callback
typedef void (*background_handler)(BackgroundScroller*);

// Background scrolling handler
class BackgroundScroller
{
public:
	BackgroundScroller(unsigned short* bg, Fixed _x, Fixed _y, Fixed sscale, Fixed dscale, int bgHandleID);
	~BackgroundScroller();
	void draw();
	void update();
	Fixed x;
	Fixed y;
	Fixed dx;
	Fixed dy;
	unsigned short* img;
	Fixed w;
	Fixed h;
	unsigned short colorKey;
	Fixed scrollScale;
	Fixed displayScale;
	background_handler handle;
};
