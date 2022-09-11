#pragma once

#include <stdint.h>

// Single key presses
typedef uint8_t t_key;

#ifndef N2DLIB_FIXED_BITS
// Define this on your own to control the format
// of the fixed-point type.
#define N2DLIB_FIXED_BITS 8
#endif

// Fixed point numbers. The position of the fixed point
// is controlled by the N2DLIB_FIXED_BITS define. If
// undefined, defaults to 24.8 fixed point.
typedef int32_t Fixed;

typedef struct
{
	int x, y, w, h;
} Rect;
