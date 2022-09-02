#pragma once

#include "globals.h"
#include "types.h"
#include "utils.hpp"
#include "graphics/BackgroundScroller.hpp"
#include "graphics/Camera.hpp"
#include "n2DLib/n2DLib.h"
#include "n2DLib/n2DLib_math.h"

// Camera travelings

void cthIntro1(Camera* cam)
{
	// None needed in level 1
	UNUSED(cam);
}

void cthChap1(Camera* cam)
{
	UNUSED(cam);
}

void cthIntro2(Camera* cam)
{
	UNUSED(cam);
}

void cthChap2(Camera* cam)
{
	if (!(G_gpTimer % 3))
	{
		cam->absY--;
	}
}

void cthChap2_2(Camera* cam)
{
	if (!(G_gpTimer % 3))
	{
		cam->absY--;
	}
	G_minX += G_minX < 60;
	G_maxX = 320 - G_minX;
	fillRect(0, 0, G_minX, 240, 0);
	fillRect(G_maxX, 0, 320 - G_maxX, 240, 0);
}

void cthChap2_boss(Camera* cam)
{
	G_minX -= G_minX > 0;
	G_maxX = 320 - G_minX;
	if (G_minX > 0)
	{
		fillRect(0, 0, G_minX, 240, 0);
		fillRect(G_maxX, 0, 320 - G_maxX, 240, 0);
	}
}

// Background travelings
// TODO : not convinced we actually need those

void cb_bgHandle_default(BackgroundScroller* bg)
{
	bg->dy += itofix(1);
}

void cb_bgHandle_2_2(BackgroundScroller* bg)
{
	bg->dy += itofix(1);
	// Here, the background scrolling is reset to 0 every time a period is completed to avoid rounding errors
	// The formula is
	// period = itofix(background_2_4.width) / background_2_4.speed
	if (bg->dy >= itofix(120 * 256 / 192)) // might as well avoid using fixdiv
		bg->dy = 0;
}
