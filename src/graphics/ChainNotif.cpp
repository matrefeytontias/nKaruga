#include "graphics/ChainNotif.hpp"

#include "n2DLib/n2DLib.hpp"

ChainNotif::ChainNotif() : Entity()
{
	untilDeath = 0;
}

ChainNotif::~ChainNotif()
{
	
}

void ChainNotif::activate(int _x, int _y, int _v)
{
	x = _x;
	backupX = x;
	y = _y;
	value = _v;
	untilDeath = 16;
	maxChain = value == 25600;
}

void ChainNotif::handle()
{
	if(untilDeath)
	{
		if(maxChain)
		{
			y -= 8;
			drawString(&x, &y, x, "max chain\n", 0xffff, 0);
		}
		drawDecimal(&x, &y, value, 0xfbe0, 0xf800);
		x = backupX;
		untilDeath--;
	}
}
