#pragma once

#include "types.h"

class Enemy;

#define UNUSED(var) (void)var

Enemy* findNearestEnemy(Fixed x, Fixed y);
bool collidePointRect(Fixed, Fixed, Fixed, Fixed, int, int);
int distance(int x1, int y1, int x2, int y2);
KeyEvent getk();
// TODO : those should go in Camera.*pp
// and/or take Constants::CamRelation
int iToScreenX(int x, int camRelation);
int iToScreenY(int y, int camRelation);
Fixed fToScreenX(Fixed x, int camRelation);
Fixed fToScreenY(Fixed y, int camRelation);
