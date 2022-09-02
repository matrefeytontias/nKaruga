#include "handlers/DrawingCandidates.hpp"

#include "globals.h"
#include "utils.hpp"
#include "helpers/Constants.hpp"
#include "n2DLib/n2DLib.hpp"

DrawingCandidates::DrawingCandidates()
{
	candidatesCount = 0;
	cam.absX = cam.absY = cam.relX = cam.relY = 0;
	cameraPath = LUTs::camTraveling(LUTs::CamTravelingId::CameraPath_c1);
}

DrawingCandidates::~DrawingCandidates()
{
	
}

void DrawingCandidates::add(unsigned short *img, Rect *pos, bool flash, int camRel)
{
	data[candidatesCount].activate(img, pos, flash, camRel);
	candidatesCount++;
}

void DrawingCandidates::add(unsigned short *img, Rect *pos, Rect *center, Fixed angle, bool flash, int camRel)
{
	data[candidatesCount].activate(img, pos, center, angle, flash, camRel);
	candidatesCount++;
}

void DrawingCandidates::flush()
{
	for(int i = 0; i < candidatesCount; i++)
	{
		data[i].draw();
		data[i].deactivate();
	}

	candidatesCount = 0;
	// Update camera
	(cameraPath)(&cam);
}

// TODO : LUTs::CamTravelingId id
void DrawingCandidates::loadCameraPath(int id)
{
	cameraPath = LUTs::camTraveling(static_cast<LUTs::CamTravelingId>(id));
}