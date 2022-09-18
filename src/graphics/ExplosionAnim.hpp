#pragma once

#include "entities/Entity.hpp"

// Explosion animations
class ExplosionAnim : public Entity
{
public:
	ExplosionAnim();
	void activate(int x, int y, bool polarity);
	void handle();
private:
	int counter;
	bool polarity;
};
