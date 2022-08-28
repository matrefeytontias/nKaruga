#include "handlers/DrawingCandidates.hpp"

#include "globals.h"
#include "utils.hpp"
#include "n2DLib/n2DLib.hpp"

//
// Camera travelling handlers
//
void cthIntro1(Camera *cam)
{
	// None needed in level 1
	UNUSED(cam);
}

void cthChap1(Camera *cam)
{
	UNUSED(cam);
}

void cthIntro2(Camera *cam)
{
	UNUSED(cam);
}

void cthChap2(Camera *cam)
{
	if(!(G_gpTimer % 3))
	{
		cam->absY--;
	}
}

void cthChap2_2(Camera *cam)
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

void cthChap2_boss(Camera *cam)
{
	G_minX -= G_minX > 0;
	G_maxX = 320 - G_minX;
	if (G_minX > 0)
	{
		fillRect(0, 0, G_minX, 240, 0);
		fillRect(G_maxX, 0, 320 - G_maxX, 240, 0);
	}
}

camera_travelling camTrav[] = { cthIntro1, cthChap1, cthIntro2, cthChap2, cthChap2_2, cthChap2_boss };

// 
// DrawingCandidates class
// 

DrawingCandidates::DrawingCandidates()
{
	candidatesCount = 0;
	cam.absX = cam.absY = cam.relX = cam.relY = 0;
	cameraPath = camTrav[0];
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

void DrawingCandidates::loadCameraPath(int id)
{
	cameraPath = camTrav[id];
}