#pragma once

#include "types.h"
#include "helpers/Constants.hpp"

class DrawingCandidate
{
public:
	DrawingCandidate();
	void activate(const unsigned short* img, const Rect* pos, bool flash, Constants::CamRelation camRelation);
	void activate(const unsigned short* img, const Rect* pos, const Rect* center, Fixed angle, bool flash, Constants::CamRelation camRelation);
	void deactivate();
	void draw();
private:
	Rect pos, center;
	bool rotates, centered, active, flash;
	// How the camera affects the object's display
	// See the enum lower in the file
	Constants::CamRelation camRelation;
	Fixed angle;
	const unsigned short* img;
};
