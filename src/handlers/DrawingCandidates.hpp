#pragma once

#include "graphics/Camera.hpp"
#include "graphics/DrawingCandidate.hpp"
#include "types.h"

#define MAX_DISPLAYABLE 2000

class DrawingCandidates
{
public:
	DrawingCandidates();
	~DrawingCandidates();
	void add(unsigned short* img, Rect* pos, bool flash, int camRelation);
	void add(unsigned short* img, Rect* pos, Rect* center, Fixed angle, bool flash, int camRelation);
	void flush();
	void loadCameraPath(int id);
	Camera cam;
private:
	DrawingCandidate data[MAX_DISPLAYABLE];
	int candidatesCount;
	camera_traveling cameraPath;
};
