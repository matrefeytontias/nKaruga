#pragma once

#include "graphics/Camera.hpp"
#include "graphics/DrawingCandidate.hpp"
#include "helpers/Constants.hpp"
#include "types.h"

#define MAX_DISPLAYABLE 2000

class DrawingCandidates
{
public:
	DrawingCandidates();
	void add(const unsigned short* img, const Rect* pos, bool flash, Constants::CamRelation camRelation);
	void add(const unsigned short* img, const Rect* pos, const Rect* center, Fixed angle, bool flash, Constants::CamRelation camRelation);
	void flush();
	void loadCameraPath(LUTs::CamTravelingId id);
	Camera cam;
private:
	DrawingCandidate data[MAX_DISPLAYABLE];
	int candidatesCount;
	camera_traveling cameraPath;
};
