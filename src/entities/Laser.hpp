#pragma once

#include "entities/Bullet.hpp"

class Enemy;

// Only enemies can fire them
class Laser : public Bullet
{
public:
	Laser();
	void activate(const Enemy* origin, bool polarity, Fixed angOffset);
	void handle();
	void draw() const;
	// (x, y) is origin
	// (w, h) are the vector's coordinates
	void getVector(Rect* vector) const;
	void getSides(Rect* side1, Rect* side2) const;
	int getAmplitude() const;
	void setAmplitude(int amp);
	Fixed angle;
	const Enemy* origin;
private:
	// Lasers are not immediate, they grow
	Fixed amplitude;
	Fixed angleOffset;
};
