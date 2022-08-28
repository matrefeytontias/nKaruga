#pragma once

#include "entities/Bullet.hpp"

class Enemy;

// Only enemies can fire them
class Laser : public Bullet
{
public:
	Laser();
	~Laser();
	void activate(Enemy* origin, bool polarity, Fixed angOffset);
	void handle();
	void draw();
	Rect* getVector();
	void getSides(Rect* side1, Rect* side2);
	int getAmplitude();
	void setAmplitude(int amp);
	Fixed angle;
	Enemy* origin;
private:
	// Lasers are not immediate, they grow
	Fixed amplitude;
	Fixed angleOffset;
};
