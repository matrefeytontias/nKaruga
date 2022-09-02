#include "helpers/math.hpp"

void rotate(int x, int y, int cx, int cy, Fixed angle, Rect* out)
{
	x -= cx;
	y -= cy;
	out->x = fixtoi(fixmul(itofix(x), fixcos(angle)) + fixmul(itofix(y), fixsin(angle))) + cx;
	out->y = fixtoi(fixmul(itofix(x), -fixsin(angle)) + fixmul(itofix(y), fixcos(angle))) + cy;
}

void getBoundingBox(int x, int y, int w, int h, int cx, int cy, Fixed angle, Rect* out)
{
	Rect tl, tr, bl, br;
	rotate(x, y, cx, cy, angle, &tl);
	rotate(x + w, y, cx, cy, angle, &tr);
	rotate(x, y + h, cx, cy, angle, &bl);
	rotate(x + w, y + h, cx, cy, angle, &br);
	out->x = min(min(min(tl.x, tr.x), bl.x), br.x);
	out->y = min(min(min(tl.y, tr.y), bl.y), br.y);
	out->w = max(max(max(tl.x, tr.x), bl.x), br.x) - out->x;
	out->h = max(max(max(tl.y, tr.y), bl.y), br.y) - out->y;
}

// Uses Lagrange's interpolation polynomial
// Returns the next t parameter so it can be fed back to the function the next call
int interpolatePathFloat(int curT, float _x[], float _y[], int _t[], int nb, Rect* out)
{
	float factor, rx = 0., ry = 0.;
	int i, j;

	for (i = 0; i < nb; i++)
	{
		factor = 1.;
		for (j = 0; j < nb; j++)
			if (i != j)
				factor *= (float)(curT - _t[j]) / (_t[i] - _t[j]);
		rx += _x[i] * factor;
		ry += _y[i] * factor;
	}

	out->x = (int)rx;
	out->y = (int)ry;
	return curT + 1;
}
