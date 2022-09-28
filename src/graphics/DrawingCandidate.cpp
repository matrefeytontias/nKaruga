#include "graphics/DrawingCandidate.hpp"

#include "utils.hpp"
#include "n2DLib/n2DLib.h"

DrawingCandidate::DrawingCandidate()
{
	
}

void DrawingCandidate::activate(const unsigned short *_img, const Rect *_pos, bool _flash, Constants::CamRelation _camRel)
{
	img = _img;
	pos.x = _pos->x - img[0] / 2;
	pos.y = _pos->y - img[1] / 2;
	camRelation = _camRel;
	rotates = false;
	active = true;
	flash = _flash;
}

void DrawingCandidate::activate(const unsigned short *_img, const Rect *_pos, const Rect *_center, Fixed _angle, bool _flash, Constants::CamRelation _camRel)
{
	img = _img;
	angle = _angle;
	pos.x = _pos->x;
	pos.y = _pos->y;
	if(_center)
	{
		center.x = _center->x;
		center.y = _center->y;
		centered = false;
	}
	else
		centered = true;
	camRelation = _camRel;
	rotates = true;
	active = true;
	flash = _flash;
}

void DrawingCandidate::deactivate()
{
	active = false;
}

void DrawingCandidate::draw()
{
	if(active)
	{
		pos.x = iToScreenX(pos.x, camRelation);
		pos.y = iToScreenY(pos.y, camRelation);
		if(rotates)
		{
			if(centered)
			{
				center.x = iToScreenX(center.x, camRelation);
				center.y = iToScreenY(center.y, camRelation);
			}
			n2D_drawSpriteRotated(img, &pos, centered ? NULL : &center, angle, flash, 0xffff);
		}
		else
			n2D_drawSprite(img, pos.x, pos.y, flash, 0xffff);
	}
}
