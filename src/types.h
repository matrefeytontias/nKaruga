#pragma once

#include <stdint.h>

class Enemy;
class BackgroundScroller;
struct Camera;

// Single key presses
typedef uint8_t t_key;

// Keypress bitfields
typedef uint32_t KeyEvent;

// 24.8 fixed-point numbers
typedef int32_t Fixed;

typedef struct Rect
{
	int x, y, w, h;
} Rect;

// Enemy pattern, describing an enemy's behaviour.
using enemy_pattern = void(*)(Enemy*);
// Background traveling, describing the way the background scrolls
// through a level or a part of a level.
using background_traveling = void(*)(BackgroundScroller*);
// Camera traveling, describing the camera's movement through
// a level or part of a level.
using camera_traveling = void(*)(Camera*);
