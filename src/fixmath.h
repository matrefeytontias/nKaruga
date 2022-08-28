#pragma once

#include "types.h"

#if defined(__cplusplus) && !defined(min) && !defined(max)
int min(int a, int b);
int max(int a, int b);
#endif
int clamp(int x, int a, int b);
int sign(int x);

Fixed itofix(int x);
int fixtoi(Fixed x);
Fixed fixmul(Fixed x, Fixed y);
Fixed fixdiv(Fixed x, Fixed y);
Fixed fixcos(Fixed angle);
Fixed fixsin(Fixed angle);
Fixed fixsq(Fixed x);
Fixed fixcube(Fixed x);
