#pragma once

#include <n2DLib/n2DLib.h>
#include <n2DLib/n2DLib_math.h>

#include "GameSystems.hpp"
#include "types.h"
#include "utils.hpp"
#include "graphics/BackgroundScroller.hpp"
#include "graphics/Camera.hpp"

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
	if (!(GS->chapterTimer % 3))
	{
		cam->absY--;
	}
}

void cthChap2_2(Camera* cam)
{
	if (!(GS->chapterTimer % 3))
	{
		cam->absY--;
	}
	GS->minX += GS->minX < 60;
	GS->maxX = 320 - GS->minX;
	n2D_fillRect(0, 0, GS->minX, 240, 0);
	n2D_fillRect(GS->maxX, 0, 320 - GS->maxX, 240, 0);
}

void cthChap2_boss(Camera* cam)
{
	GS->minX -= GS->minX > 0;
	GS->maxX = 320 - GS->minX;
	if (GS->minX > 0)
	{
		n2D_fillRect(0, 0, GS->minX, 240, 0);
		n2D_fillRect(GS->maxX, 0, 320 - GS->maxX, 240, 0);
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
