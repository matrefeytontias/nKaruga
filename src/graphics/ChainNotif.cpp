#include "graphics/ChainNotif.hpp"

#include "n2DLib/n2DLib.h"

ChainNotif::ChainNotif() : Entity()
{
	untilDeath = 0;
}

void ChainNotif::activate(int _x, int _y, int _v)
{
	Entity::activate();
	x = _x;
	y = _y;
	value = _v;
	untilDeath = 16;
	maxChain = value == 25600;
}

void ChainNotif::handle()
{
	if(maxChain)
	{
		y -= 8;
		drawString(&x, &y, x, "max chain\n", 0xffff, 0);
	}
	int tempX = x;
	drawDecimal(&tempX, &y, value, 0xfbe0, 0xf800);
	untilDeath--;

	if (!untilDeath)
		deactivate();
}
