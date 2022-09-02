#pragma once

#include "types.h"
#include "entities/Enemy.hpp"
#include "entities/BossEnemy.hpp"

#define UNUSED(var) (void)var

Enemy* findNearestEnemy(Fixed x, Fixed y);
bool collidePointRect(Fixed, Fixed, Fixed, Fixed, int, int);
int distance(int x1, int y1, int x2, int y2);
KeyEvent getk();
int iToScreenX(int x, int camRel);
int iToScreenY(int y, int camRel);
Fixed fToScreenX(Fixed x, int camRel);
Fixed fToScreenY(Fixed y, int camRel);
