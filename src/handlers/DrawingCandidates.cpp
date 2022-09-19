﻿#include "handlers/DrawingCandidates.hpp"

#include "GameSystems.hpp"
#include "utils.hpp"
#include "helpers/Constants.hpp"
#include "n2DLib/n2DLib.h"

DrawingCandidates::DrawingCandidates()
{
	candidatesCount = 0;
	cam.absX = cam.absY = cam.relX = cam.relY = 0;
	cameraPath = nullptr;
}

void DrawingCandidates::add(const unsigned short *img, const Rect *pos, bool flash, Constants::CamRelation camRelation)
{
	data[candidatesCount].activate(img, pos, flash, camRelation);
	candidatesCount++;
}

void DrawingCandidates::add(const unsigned short *img, const Rect *pos, const Rect *center, Fixed angle, bool flash, Constants::CamRelation camRelation)
{
	data[candidatesCount].activate(img, pos, center, angle, flash, camRelation);
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
	if(cameraPath != nullptr)
		(cameraPath)(&cam);
}

// TODO : LUTs::CamTravelingId id
void DrawingCandidates::loadCameraPath(LUTs::CamTravelingId id)
{
	cameraPath = LUTs::camTraveling(id);
}
