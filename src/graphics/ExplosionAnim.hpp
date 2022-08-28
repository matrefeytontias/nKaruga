#pragma once

#include "entities/Entity.hpp"

// Explosion animations
class ExplosionAnim : public Entity
{
public:
	ExplosionAnim();
	~ExplosionAnim();
	void activate(int, int, bool);
	void handle();
private:
	int counter;
	bool polarity;
};
