#pragma once

#include <n2DLib/n2DLib_types.h>

typedef int16_t KeyEvent;

class Enemy;
class BackgroundScroller;
struct Camera;

// Enemy pattern, describing an enemy's behaviour.
typedef void(*enemy_pattern)(Enemy*);
// Background traveling, describing the way the background scrolls
// through a level or a part of a level.
typedef void(*background_traveling)(BackgroundScroller*);
// Camera traveling, describing the camera's movement through
// a level or part of a level.
typedef void(*camera_traveling)(Camera*);
