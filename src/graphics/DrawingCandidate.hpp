#pragma once

#include "types.h"

class DrawingCandidate
{
public:
	DrawingCandidate();
	~DrawingCandidate();
	void activate(unsigned short* img, Rect* pos, bool flash, int camRelation);
	void activate(unsigned short* img, Rect* pos, Rect* center, Fixed angle, bool flash, int camRelation);
	void deactivate();
	void draw();
private:
	Rect pos, center;
	bool rotates, centered, active, flash;
	// How the camera affects the object's display
	// See the enum lower in the file
	int camRel;
	Fixed angle;
	unsigned short* img;
};
