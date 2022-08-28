#pragma once

#include <stdint.h>

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
