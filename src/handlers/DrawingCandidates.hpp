#pragma once

#include "graphics/DrawingCandidate.hpp"

#define MAX_DISPLAYABLE 2000

typedef struct
{
	int absX, absY;
	int relX, relY;
} Camera;

typedef void (*camera_travelling)(Camera*);

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
	camera_travelling cameraPath;
};
