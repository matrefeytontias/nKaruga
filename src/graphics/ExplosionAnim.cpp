#include "graphics/ExplosionAnim.hpp"

#include "GameSystems.hpp"
#include "helpers/Constants.hpp"
#include "n2DLib/n2DLib.h"

#define FRAME_DELAY 4
// TODO : cache this once and for all
#define EXPLOSION_W LUTs::baseImage(LUTs::BaseImageId::EXPLOSION_LIGHT_0)[0]
#define EXPLOSION_H LUTs::baseImage(LUTs::BaseImageId::EXPLOSION_LIGHT_0)[1]

ExplosionAnim::ExplosionAnim() : Entity()
{
	counter = 0;
}

ExplosionAnim::~ExplosionAnim()
{
	
}

void ExplosionAnim::activate(int _x, int _y, bool _p)
{
	x = _x - EXPLOSION_W / 2;
	y = _y - EXPLOSION_H / 2;
	polarity = _p;
	counter = 6 * FRAME_DELAY;
}

void ExplosionAnim::handle()
{
	if(counter)
	{
		// Isn't a drawing candidate
		drawSprite(LUTs::baseImage(polarity ? LUTs::BaseImageId::EXPLOSION_SHADOW_0 : LUTs::BaseImageId::EXPLOSION_LIGHT_0,
								   (6 * FRAME_DELAY - counter) / FRAME_DELAY),
				   x, y, 0, 0);
		counter--;
	}
}
