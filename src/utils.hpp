#pragma once

#include "types.h"

class Enemy;

#define UNUSED(var) (void)var

// KeyEvent
#define KDOWN(x) (x & 1)
#define KLEFT(x) (x & 2)
#define KRIGHT(x) (x & 4)
#define KUP(x) (x & 8)
#define KFIRE(x) (x & 16)
#define KPOLARITY(x) (x & 32)
#define KFRAGMENT(x) (x & 64)
#define KQUIT(x) (x & 128)
#define KPAUSE(x) (x & 256)

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
