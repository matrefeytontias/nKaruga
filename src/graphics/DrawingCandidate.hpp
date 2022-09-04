#pragma once

#include "types.h"
#include "helpers/Constants.hpp"

class DrawingCandidate
{
public:
	DrawingCandidate();
	~DrawingCandidate();
	void activate(unsigned short* img, Rect* pos, bool flash, Constants::CamRelation camRelation);
	void activate(unsigned short* img, Rect* pos, Rect* center, Fixed angle, bool flash, Constants::CamRelation camRelation);
	void deactivate();
	void draw();
private:
	Rect pos, center;
	bool rotates, centered, active, flash;
	// How the camera affects the object's display
	// See the enum lower in the file
	Constants::CamRelation camRelation;
	Fixed angle;
	unsigned short* img;
};
