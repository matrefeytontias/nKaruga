#pragma once

#include "types.h"

#if defined(__cplusplus) && !defined(min) && !defined(max)

constexpr int min(int a, int b)
{
	return a < b ? a : b;
}

constexpr int max(int a, int b)
{
	return a > b ? a : b;
}

#endif

constexpr int clamp(int x, int a, int b)
{
	return min(max(x, a), b);
}

constexpr int sign(int x)
{
	return x < 0;
}

constexpr int sq(int x)
{
	return x * x;
}

constexpr int cube(int x)
{
	return x * x * x;
}

constexpr Fixed itofix(int x)
{
	return x << 8;
}

constexpr int fixtoi(Fixed x)
{
	return x >> 8;
}

constexpr Fixed fixmul(Fixed x, Fixed y)
{
	// x = (xint << 8) + xdec, y = (yint << 8) + ydec
	Fixed xdec = x & 0xff, ydec = y & 0xff, xint = x >> 8, yint = y >> 8;
	// Like (x * y) >> 8 ; a bit slower but without any risk of overflow (noticeable when squaring and cubing)
	return ((xint * yint) << 8) + xint * ydec + xdec * yint + ((xdec * ydec) >> 8);
}

constexpr Fixed fixdiv(Fixed x, Fixed y)
{
	// TODO : prevent overflow ?
	return (x << 8) / y;
}

constexpr Fixed fixcos(Fixed angle)
{
	constexpr Fixed const cosLUT[] = { 256, 255, 255, 255, 254, 254, 253, 252, 251, 249, 248, 246, 244, 243, 241, 238, 236, 234, 231, 228, 225, 222, 219, 216, 212, 209, 205, 201, 197, 193, 189, 185, 181, 176, 171, 167, 162, 157, 152, 147, 142, 136, 131, 126, 120, 115, 109, 103, 97, 92, 86, 80, 74, 68, 62, 56, 49, 43, 37, 31, 25, 18, 12, 6, 0, -6, -12, -18, -25, -31, -37, -43, -49, -56, -62, -68, -74, -80, -86, -92, -97, -103, -109, -115, -120, -126, -131, -136, -142, -147, -152, -157, -162, -167, -171, -176, -181, -185, -189, -193, -197, -201, -205, -209, -212, -216, -219, -222, -225, -228, -231, -234, -236, -238, -241, -243, -244, -246, -248, -249, -251, -252, -253, -254, -254, -255, -255, -255, -256, -255, -255, -255, -254, -254, -253, -252, -251, -249, -248, -246, -244, -243, -241, -238, -236, -234, -231, -228, -225, -222, -219, -216, -212, -209, -205, -201, -197, -193, -189, -185, -181, -176, -171, -167, -162, -157, -152, -147, -142, -136, -131, -126, -120, -115, -109, -103, -97, -92, -86, -80, -74, -68, -62, -56, -49, -43, -37, -31, -25, -18, -12, -6, 0, 6, 12, 18, 25, 31, 37, 43, 49, 56, 62, 68, 74, 80, 86, 92, 97, 103, 109, 115, 120, 126, 131, 136, 142, 147, 152, 157, 162, 167, 171, 176, 181, 185, 189, 193, 197, 201, 205, 209, 212, 216, 219, 222, 225, 228, 231, 234, 236, 238, 241, 243, 244, 246, 248, 249, 251, 252, 253, 254, 254, 255, 255, 255 };
	return cosLUT[angle & 0xff];
}

constexpr Fixed fixsin(Fixed angle)
{
	return fixcos(angle - 64);
}

constexpr Fixed fixsq(Fixed x)
{
	return fixmul(x, x);
}

constexpr Fixed fixcube(Fixed x)
{
	return fixmul(fixmul(x, x), x);
}

void rotate(int x, int y, int cx, int cy, Fixed angle, Rect* out);
void getBoundingBox(int x, int y, int w, int h, int cx, int cy, Fixed angle, Rect* out);
int interpolatePathFloat(int, float[], float[], int[], int, Rect*);