#pragma once

#include "n2DLib/n2DLib_types.h"

#ifdef __cplusplus
extern "C" {
#endif

#if !defined(min) && !defined(max)

int min(int a, int b);
int max(int a, int b);

#endif

int clamp(int x, int a, int b);
int sign(int x);
int sq(int x);
int cube(int x);

Fixed itofix(int x);
int fixtoi(Fixed x);
Fixed fixmul(Fixed x, Fixed y);
Fixed fixdiv(Fixed x, Fixed y);
Fixed fixcos(Fixed angle);
Fixed fixsin(Fixed angle);
Fixed fixsq(Fixed x);
Fixed fixcube(Fixed x);

void rotate(int x, int y, int cx, int cy, Fixed angle, Rect* out);
void getBoundingBox(int x, int y, int w, int h, int cx, int cy, Fixed angle, Rect* out);
int interpolatePathFloat(int, float[], float[], int[], int, Rect*);

#ifdef __cplusplus
}
#endif
