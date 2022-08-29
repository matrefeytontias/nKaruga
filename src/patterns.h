#pragma once

#include "common.h"
#include "utils.hpp"
#include "handlers/Level.hpp"
#include "helpers/Constants.hpp"
#include "helpers/math.hpp"

#define cb_args Enemy *e

void cb_Pattern_null(cb_args)
{
	
}

void cb_Pattern_box(cb_args)
{
	e->beAbox();
}

void cb_Pattern_prop(cb_args)
{
	e->beAprop();
}

// #######
// Intro 1
// #######
void cb_Pattern_1_1(cb_args)
{
	e->internal[1] = e->internal[0];
	e->setx(itofix(320) - fixmul(sq(e->internal[1]), 768));
	e->sety(itofix(e->internal[1]));
	e->internal[0] += 2;
	e->rotationAngle = e->internal[0];
}

void cb_Pattern_1_2(cb_args)
{
	e->internal[1] = e->internal[0];
	e->setx(fixmul(sq(e->internal[1]), 768));
	e->sety(itofix(e->internal[1]));
	e->internal[0] += 2;
	e->rotationAngle = e->internal[0];
}
void cb_Pattern_1_3(cb_args)
{
	Rect screenRect;
	e->setAX(3, 320.0f, 160.0f + static_cast<float>(e->waveIndex * e->img[0]) / 2.0f, 320.0f);
	e->setAY(3, 0.0f, 120.0f, 240.0f);
	e->internal[1] = (5 - e->waveIndex) * 16 * 2 + 120;
	e->setAT(3, 0, (5 - e->waveIndex) * 16 + 60, e->internal[1]);
	e->internal[0] = interpolatePathFloat(e->internal[0], e->ax, e->ay, e->at, 3, &screenRect);
	e->setx(itofix(screenRect.x));
	e->sety(itofix(screenRect.y));
	e->rotationAngle = e->internal[0];
	if (e->internal[0] == e->internal[1]) e->deactivate();
}

void cb_Pattern_1_4(cb_args)
{
	Rect screenRect;
	e->setAX(3, 0.0f, 160.0f - static_cast<float>((e->waveIndex - 6) * e->img[0]) / 2.0f, 0.0f);
	e->setAY(3, 0.0f, 120.0f, 240.0f);
	e->internal[1] = (5 - (e->waveIndex - 6)) * 16 * 2 + 120;
	e->setAT(3, 0, (5 - (e->waveIndex - 6)) * 16 + 60, e->internal[1]);
	e->internal[0] = interpolatePathFloat(e->internal[0], e->ax, e->ay, e->at, 3, &screenRect);
	e->setx(itofix(screenRect.x));
	e->sety(itofix(screenRect.y));
	e->rotationAngle = e->internal[0] / 2;
	if (e->internal[0] == e->internal[1]) e->deactivate();
}

void cb_Pattern_1_5(cb_args)
{
	if (!(Level::waveTimer % 2))
	{
		//if (Level::waveTimer < 250)
		if (fixtoi(e->gety()) < 120 - (e->waveIndex - 12) / 6 * 12)
			e->addy(itofix(1));
		else
			e->addx((e->waveIndex % 6) > 2 ? 384 : -384);
	}
}

void cb_Pattern_1_6(cb_args)
{
	Fixed angle;
	angle = e->angleToEntity(Level::p);
	if (!(e->internal[0] % 25))
	{
		Level::bArray->add(e->getx(), e->gety(), angle, itofix(1), static_cast<int>(LUTs::BaseImageId::ENEMY_BULLET_1_LIGHT), e->polarity, true, e->getCamRel());
	}
	e->internal[0]++;
	e->rotationAngle = ~angle + 64;
}

// #########
// Chapter 1
// #########
void cb_Pattern_1_7(cb_args)
{
	if (!(Level::waveTimer % 2))
	{
		e->addy(abs(fixcos(e->internal[0])) * 3);
		e->internal[0]++;
		if (e->internal[0] < 100)
			e->addx(e->getx() > itofix(160) ? -128 : 128);
		else
			e->addx(e->getx() > itofix(160) ? itofix(1) : itofix(-1));
	}
}

void cb_Pattern_1_8(cb_args)
{
	Fixed angle;
	if (!(Level::waveTimer % 2))
	{
		angle = e->angleToEntity(Level::p);
		if (e->internal[0] < 35)
			e->addy(itofix(2));
		else
			e->rotationAngle = ~angle + 64;
		if (e->internal[0] > 65 && e->internal[0] < 100)
		{
			if (!(Level::waveTimer % 8))
				Level::bArray->add(e->getx(), e->gety(), angle, itofix(2), static_cast<int>(LUTs::BaseImageId::ENEMY_BULLET_1_LIGHT), e->polarity, true, e->getCamRel());
		}
		else if (e->internal[0] > 164)
		{
			e->addx(e->getx() < itofix(160) ? itofix(-1) : itofix(1));
			e->suby(itofix(1));
		}
		e->internal[0]++;
	}
}

void cb_Pattern_1_9(cb_args)
{
	e->addx((e->waveIndex % 2) ? 384 : -384);
	e->addy(256);

	if (!(Level::waveTimer % 4))
		Level::bArray->add(e->getx(), e->gety(), 64, itofix(4), static_cast<int>(LUTs::BaseImageId::ENEMY_BULLET_1_LIGHT), e->polarity, true, e->getCamRel());
}

void cb_Pattern_1_10(cb_args)
{
	if (!(Level::waveTimer % 2))
	{
		int temp = fixsin(e->internal[0]) * 80;
		e->setx(((e->waveIndex % 2) ? -temp : temp) + itofix(160));
		if (abs(temp) > itofix(70))
		{
			if (!(Level::waveTimer % 8))
				Level::bArray->add(e->getx(), e->gety(), 64, itofix(4), static_cast<int>(LUTs::BaseImageId::ENEMY_BULLET_1_LIGHT), e->polarity, true, e->getCamRel());
		}
		e->addy(384);
		e->internal[0]++;
	}
}

void cb_Pattern_1_11(cb_args)
{
	e->addy(itofix(2));
}

void cb_Pattern_1_12(cb_args)
{
	Fixed angle;
	if (!(Level::waveTimer % 2))
	{
		if (e->internal[0] < 45)
			e->addy(itofix(2));
		else if (e->internal[0] == 45)
		{
			e->internal[1] = (e->waveIndex % 2) ? 0 : 128;
			e->internal[2] = e->getx() + ((e->waveIndex % 2) ? itofix(-20) : itofix(20));
			e->internal[3] = e->gety();
		}
		else if (e->internal[0] - 45 < 416)
		{
			e->setx(fixcos(e->internal[1]) * 20 + e->internal[2]);
			e->sety(fixsin(e->internal[1]) * 20 + e->internal[3]);
			e->internal[4] = (e->internal[0] - 45) % 64;
			if (e->internal[4] >= 32)
			{
				if (!(e->internal[1] % 256))
				{
					angle = e->angleToEntity(Level::p);
					Level::bArray->add(e->getx(), e->gety(), angle, itofix(2), static_cast<int>(LUTs::BaseImageId::ENEMY_BULLET_0_LIGHT), e->polarity, true, e->getCamRel());
				}
				e->internal[1] += 4;
			}
		}
		else
			e->addx(e->getx() < itofix(160) ? itofix(-2) : itofix(2));

		e->internal[0]++;
		e->rotationAngle = e->internal[0];
	}
}

void cb_Pattern_1_13(cb_args)
{
	e->rotationAngle = 16 + fixcos(Level::waveTimer) / 16;
	if (!e->internal[1])
	{
		e->internal[1] = 1;
		e->internal[0] = -1;
	}
	if (e->gety() < itofix(30))
	{
		e->addy(256);
	}
	else if (e->internal[2] < 3)
	{
		if (e->internal[0] > 2 && e->internal[0] < 12)
		{
			if (!(Level::waveTimer % 2))
			{
				for (int i = 0; i < 4; i++)
				{
					int k = i * 8 - 20 + 48;
					Level::bArray->add(fixcos(k + (e->internal[0] - 3) * 8) * e->img[0] / 2 + e->getx(), fixsin(k + (e->internal[0] - 3) * 8) * e->img[1] / 2 + e->gety(),
						k, (e->internal[0] - 2) * 64 + itofix(1), static_cast<int>(LUTs::BaseImageId::ENEMY_BULLET_0_LIGHT), e->polarity, true, e->getCamRel());
				}
				e->internal[0]++;
			}
		}
		else if (e->internal[0] == 12)
		{
			e->internal[0] = 0;
			e->internal[2]++;
		}
		else
		{
			if (!(Level::waveTimer % 64))
			{
				if (e->internal[0] < 2 && e->internal[0] >= 0)
				{
					for (int i = 0; i < 5; i++)
					{
						int k = (i - 2) * 16 + 48;
						for (int j = 0; j < 3; j++)
						{

							Level::bArray->add(fixcos(k) * e->img[0] / 2 + e->getx() - fixsin(k) * (j - 1) * 6, fixsin(k) * e->img[1] / 2 + e->gety() + fixcos(k) * (j - 1) * 6,
								k, itofix(2), static_cast<int>(LUTs::BaseImageId::ENEMY_BULLET_2_LIGHT), e->polarity, true, e->getCamRel());
						}
					}
				}
				e->internal[0]++;
			}
		}
	}
	else
		e->subx(128);
}

void cb_Pattern_1_14(cb_args)
{
	e->rotationAngle = -16 + fixcos(Level::waveTimer) / 16;
	if (e->gety() < itofix(30))
	{
		e->addy(256);
	}
	else if (e->internal[2] < 3)
	{
		if (e->internal[0] > 2 && e->internal[0] < 12)
		{
			if (!(Level::waveTimer % 2))
			{
				for (int i = 0; i < 4; i++)
				{
					int k = i * 8 - 20 + 80;
					Level::bArray->add(fixcos(k - (e->internal[0] - 3) * 8) * e->img[0] / 2 + e->getx(), fixsin(k - (e->internal[0] - 3) * 8) * e->img[1] / 2 + e->gety(),
						k, (e->internal[0] - 2) * 64 + itofix(1), static_cast<int>(LUTs::BaseImageId::ENEMY_BULLET_0_LIGHT), e->polarity, true, e->getCamRel());
				}
				e->internal[0]++;
			}
		}
		else if (e->internal[0] == 12)
		{
			e->internal[0] = 0;
			e->internal[2]++;
		}
		else
		{
			if (!(Level::waveTimer % 64))
			{
				if (e->internal[0] < 2)
				{
					for (int i = 0; i < 5; i++)
					{
						int k = (i - 2) * 16 + 80;
						for (int j = 0; j < 3; j++)
						{

							Level::bArray->add(fixcos(k) * e->img[0] / 2 + e->getx() - fixsin(k) * (j - 1) * 6, fixsin(k) * e->img[1] / 2 + e->gety() + fixcos(k) * (j - 1) * 6,
								k, itofix(2), static_cast<int>(LUTs::BaseImageId::ENEMY_BULLET_2_LIGHT), e->polarity, true, e->getCamRel());
						}
					}
				}
				e->internal[0]++;
			}
		}
	}
	else
		e->addx(128);
}

void cb_Pattern_1_15(cb_args)
{
	Fixed angle;
	e->setx(fixcos(e->internal[0] + ((e->waveIndex % 2) ? 128 : 0)) * 30);
	e->sety(fixsin(e->internal[0] + ((e->waveIndex % 2) ? 128 : 0)) * 30);
	if (!(Level::waveTimer % 2))
	{
		if (!(e->internal[0] % 128))
		{
			angle = e->angleToEntity(Level::p);
			Level::bArray->add(e->getx(), e->gety(), angle, itofix(1), static_cast<int>(LUTs::BaseImageId::ENEMY_BULLET_0_LIGHT), e->polarity, true, e->getCamRel());
		}
		e->internal[0]++;
		e->rotationAngle = e->internal[0];
	}
}

void cb_Pattern_1_16(cb_args)
{
	if (e->gety() <= itofix(80))
	{
		if (!(Level::waveTimer % 16) && e->gety() < itofix(60))
			Level::bArray->add(e->getx(), e->gety(), 64, itofix(3), static_cast<int>(LUTs::BaseImageId::ENEMY_BULLET_1_LIGHT), e->polarity, true, e->getCamRel());
		e->addy((256 - fixtoi(e->gety()) * 3) * 2);
		e->addx(e->polarity ? -128 : 128);
	}
	else
	{
		e->addy(128);
		e->addx(e->polarity ? -256 : 256);
	}
}

void cb_Pattern_1_17(cb_args)
{
	if (!(Level::waveTimer % 2))
	{
		// Y position is timed to be starting position + 96 due to the enemy needing to rotate by 32 degrees in a single movement
		// So it moves by 3 pixels and rotates by one degree at a time
		if (e->gety() < itofix(46) && !e->internal[0])
		{
			e->rotationAngle--;
			// 1.5
			e->addy(384);
		}
		else if (e->gety() == itofix(46) && !e->internal[0])
		{
			// FIRE DA L4Z000000000000000R
			Level::bArray->fire_laser(e, e->polarity, 64);
			e->rotationAngle++;
			e->internal[0] = 1;
			e->internal[1] = e->rotationAngle * 2;
		}
		else if (e->rotationAngle < 32)
		{
			e->internal[1]++;
			e->rotationAngle = e->internal[1] / 2;
		}
		else if (e->rotationAngle == 32)
		{
			Level::bArray->stop_laser(e);
			e->rotationAngle++;
		}
		else
		{
			e->rotationAngle++;
			e->suby(itofix(1));
		}
	}
}

void cb_Pattern_1_18(cb_args)
{
	if (!(Level::waveTimer % 2))
	{
		e->setx(fixcos(Level::waveTimer + e->waveIndex * 21) * max(200 - e->internal[0], 80) + itofix(160));
		e->sety(fixsin(Level::waveTimer + e->waveIndex * 21) * max(200 - e->internal[0], 80) + itofix(120));

		if (Level::waveTimer < 512)
			e->internal[0] += (e->internal[0] < 120) * 2;
		else
			e->internal[0] -= 2;
		e->rotationAngle += 3;
	}
}

void cb_Pattern_1_19(cb_args)
{
	if (!e->internal[3])
	{
		e->internal[3] = 1;
		e->internal[0] = e->waveIndex ? 128 : -128;
		e->internal[1] = e->waveIndex ? -128 : 128;
		e->internal[2] = -1;
	}
	if (e->internal[2] < 5)
	{
		switch (e->internal[2] % 4)
		{
		case 1:
			e->addy(e->internal[1]);
			if (e->gety() <= itofix(e->img[1] / 2) || e->gety() >= itofix(240 - e->img[1] / 2 - 15))
				e->internal[2]++;
			break;
		case 2:
			e->subx(e->internal[0]);
			if (e->getx() <= itofix(e->img[0] / 2 + 20) || e->getx() >= itofix(320 - e->img[0] / 2 - 20))
				e->internal[2]++;
			break;
		case 3:
			e->suby(e->internal[1]);
			if (e->gety() <= itofix(e->img[1] / 2) || e->gety() >= itofix(240 - e->img[1] / 2 - 15))
				e->internal[2]++;
			break;
		default:
			e->addx(e->internal[0]);
			if (e->getx() == itofix(e->img[0] / 2 + 20) || e->getx() == itofix(320 - e->img[0] / 2 - 20))
				e->internal[2]++;
		}
	}
	else
		e->addy(e->internal[1]);
}

void cb_Pattern_1_20(cb_args)
{
	if (!(Level::waveTimer % 16))
		Level::bArray->add(e->getx(), e->gety(), (e->waveIndex + 1 % 2) * 128, itofix(2), static_cast<int>(LUTs::BaseImageId::ENEMY_BULLET_2_LIGHT), e->polarity, true, e->getCamRel());
}

void cb_Pattern_1_21(cb_args)
{
	if (!e->internal[0] && e->waveIndex % 2)
		e->internal[0] = 128;
	e->addy(96);
	e->setx(fixcos(e->internal[0]) * 14 + itofix(160));
	e->internal[0] += 2;
}

void cb_Pattern_1_boss(cb_args)
{
	Fixed angle;
	if (e->gety() < itofix(20))
		e->addy(256);
	else
	{
		e->rotationAngle++;
		if (!(Level::waveTimer % 48))
		{
			angle = e->angleToEntity(Level::p);
			Level::bArray->add(e->getx(), e->gety(), angle, itofix(2), static_cast<int>(LUTs::BaseImageId::ENEMY_BULLET_1_LIGHT), e->polarity, true, e->getCamRel());
		}
	}
}

void cb_Pattern_1_bossGrenade(cb_args)
{
	// e->internal[0] is timer before explosion
	// e->internal[1] and [2] are the dx/dy to position
	// e->internal[3] is the time to wait before explosion
	if (!e->internal[0])
	{
		e->internal[3] = 128 + (rand() % 64);
		e->internal[2] = (itofix(120) - e->gety()) * 4 / e->internal[3];
		switch (e->waveIndex % 4)
		{
		case 0:
			e->internal[1] = itofix(20) - e->getx();
			break;
		case 1:
			e->internal[1] = itofix(110) - e->getx();
			break;
		case 2:
			e->internal[1] = itofix(210) - e->getx();
			break;
		default:
			e->internal[1] = itofix(300) - e->getx();
		}
		e->internal[1] /= e->internal[3] / 2;
	}
	if (e->gety() < itofix(120))
	{
		e->addx(fixmul(e->internal[1], e->internal[3] - e->internal[0]));
		e->addy(fixmul(e->internal[2], e->internal[3] - e->internal[0]));
	}
	if (e->internal[0] >= e->internal[3])
	{
		for (int i = 0; i < 32; i++)
			Level::bArray->add(e->getx(), e->gety(), i * 8, itofix(2), static_cast<int>(LUTs::BaseImageId::ENEMY_BULLET_0_LIGHT), e->polarity, true, e->getCamRel());
		e->damageable = true; // force destruction
		e->damage(!e->polarity, e->HP);
	}
	e->internal[0]++;
	e->rotationAngle++;
}


// #######
// Intro 2
// #######
void cb_Pattern_2_1(cb_args)
{
	if (!e->internal[0])
	{
		if (fixtoi(e->getx()) != 160)
		{
			e->internal[3] = (fixtoi(e->getx()) < 160) * 2 - 1;
			e->addx(itofix((e->getx() < itofix(160)) * 4 - 2));
			if (abs(fixtoi(e->getx()) - 160) == 80)
			{
				Fixed angle = e->angleToEntity(Level::p);
				Level::bArray->add(e->getx(), e->gety(), angle, itofix(2), static_cast<int>(LUTs::BaseImageId::ENEMY_BULLET_0_LIGHT), e->polarity, true, e->getCamRel());
			}
		}
		else
		{
			e->internal[0] = 1;
			e->internal[1] = 192;
			e->internal[2] = itofix(120) - e->gety();
		}
	}
	else if(e->internal[0] == 1)
	{
		e->setx(fixmul(fixcos(e->internal[1]), e->internal[2]) + itofix(160));
		e->sety(fixmul(fixsin(e->internal[1]), e->internal[2]) + itofix(120));
		if(e->internal[3] > 0)
		{
			e->internal[1]++;
			if(e->internal[1] >= 224 + 256)
			{
				e->internal[0] = 2;
				e->internal[1] = 32;
			}
		}
		else
		{
			e->internal[1]--;
			if(e->internal[1] <= 160 - 256)
			{
				e->internal[0] = 2;
				e->internal[1] = 96;
			}
		}
		e->internal[2] += 64;
	}
	else
	{
		e->addx(fixcos(e->internal[1]) * 3);
		e->addy(fixsin(e->internal[1]) * 3);
	}
	e->rotationAngle += e->internal[3];
}

void cb_Pattern_2_2(cb_args)
{
	// x = 40 or 280
	if (!e->internal[0])
	{
		if (fixtoi(e->gety()) != 120)
		{
			e->internal[3] = fixtoi(e->getx()) > 160;
			e->addy(itofix(2));
		}
		else
		{
			e->internal[0] = 1;
			e->internal[1] = e->internal[3] ? 0 : 128;
			e->internal[2] = abs(itofix(160) - e->getx());
		}
		if (fixtoi(e->getx()) == fixtoi(e->gety()) || 320 - fixtoi(e->getx()) == fixtoi(e->gety()))
		{
			Fixed angle = e->angleToEntity(Level::p);
			Level::bArray->add(e->getx(), e->gety(), angle, itofix(2), static_cast<int>(LUTs::BaseImageId::ENEMY_BULLET_0_LIGHT), e->polarity, true, e->getCamRel());
		}
	}
	else if ((e->internal[1] & 0xff) != (e->internal[3] ? 224 : 160))
	{
		e->setx(fixmul(fixcos(e->internal[1]), e->internal[2]) + itofix(160));
		e->sety(fixmul(fixsin(e->internal[1]), e->internal[2]) + itofix(120));
		e->internal[1] += e->internal[3] * 2 - 1;
		e->rotationAngle = -e->internal[1] + !e->internal[3] * 128;
	}
	else
	{
		e->addy(itofix(2));
		e->addx(itofix(e->internal[3] * 2 - 1) * 2);
	}
}

void cb_Pattern_2_3(cb_args)
{
	// y = 40
	if (!e->internal[0])
	{
		if (fixtoi(e->getx()) != 160)
		{
			e->internal[3] = fixtoi(e->getx()) < 160;
			e->addx(itofix(e->internal[3] * 2 - 1) * 2);
			e->rotationAngle = 64 * (e->internal[3] * 2 - 1);
		}
		else
		{
			e->internal[0] = 1;
			// e->internal[1] = 192 + e->internal[3] * 2 - 1;
			e->internal[1] = 192;
			e->internal[2] = abs(itofix(120) - e->gety());
		}
		if (fixtoi(e->getx()) == fixtoi(e->gety()) || 320 - fixtoi(e->getx()) == fixtoi(e->gety()))
		{
			Fixed angle = e->angleToEntity(Level::p);
			Level::bArray->add(e->getx(), e->gety(), angle, itofix(2), static_cast<int>(LUTs::BaseImageId::ENEMY_BULLET_0_LIGHT), e->polarity, true, e->getCamRel());
		}
	}
	else if (!e->internal[4])
	{
		e->setx(fixmul(fixcos(e->internal[1]), e->internal[2]) + itofix(160));
		e->sety(fixmul(fixsin(e->internal[1]), e->internal[2]) + itofix(120));
		e->internal[1] += e->internal[3] * 2 - 1;
		e->rotationAngle = -e->internal[1] + !e->internal[3] * 128;
		if (e->internal[1] == (e->internal[3] ? 512 : -128)) e->internal[4] = 1;
	}
	else
	{
		e->addy(itofix(2));
	}
}

void cb_Pattern_2_4(cb_args)
{
	if (e->getx() < itofix(120))
	{
		if (e->getx() == itofix(60))
			Level::bArray->fire_laser(e, e->polarity, 64);
		e->addx(256);
	}
	else if (e->getx() > itofix(200))
	{
		if (e->getx() == itofix(260))
			Level::bArray->fire_laser(e, e->polarity, 64);
		e->subx(256);
	}
	else if (fixtoi(e->getx()) == 120 || fixtoi(e->getx()) == 200)
	{
		if (e->internal[1] < 512)
			e->internal[1]++;
		else if (e->internal[1] == 512)
		{
			Level::bArray->stop_laser(e);
			e->internal[1]++;
		}
		else
			e->suby(256);
	}
}

void cb_Pattern_2_5(cb_args)
{
	if (e->getx() < itofix(160))
	{
		if (!e->internal[0] && e->getx() < itofix(80 - (e->waveIndex % 2) * 20))
			e->addx(256);
		else
		{
			e->internal[0]++;
			if (e->internal[0] > 384)
				e->subx(256);
		}
	}
	else
	{
		if (!e->internal[0] && e->getx() > itofix(240 + (e->waveIndex % 2) * 20))
			e->subx(256);
		else
		{
			e->internal[0]++;
			if (e->internal[0] > 384)
				e->addx(256);
		}
	}
	e->rotationAngle += ((e->waveIndex % 2) * 2 - 1) * 2;
}

void cb_Pattern_2_6(cb_args)
{
	// Those are the 14th, 15th, 16th and 17th enemies of the wave (starting e->at 0)
	if (!e->internal[0])
		e->internal[0] = e->waveIndex - 13; // avoid having 0
	if (!e->internal[1] && e->gety() < itofix(135 - (e->internal[0] - 1) * 30))
		e->addy(256);
	else
	{
		e->internal[1]++;
		if (e->internal[1] > 384)
			e->addx(itofix(2) * (((e->internal[0] - 1) % 2) * 2 - 1));
	}
	e->rotationAngle += 2;
}

void cb_Pattern_2_7(cb_args)
{
	// e starts e->at e->waveIndex = 18 -> odd parity
	// Odd parity is left, even parity is right
	if (e->gety() < itofix(135))
	{
		e->addy(itofix(2));
	}
	else
	{
		if (e->gety() == itofix(135))
			e->internal[1] = 128;
		if (e->waveIndex % 2)
			e->subx(e->internal[1]);
		else
			e->addx(e->internal[1]);
		e->internal[1] += 16;
		e->addy(256);
	}
}

void cb_Pattern_2_leftDoor(cb_args)
{
	if(!e->internal[1])
		e->internal[1] = 128;
    if(Level::phase == PHASE_TRANSITION)
    {
		if(e->getx() < itofix(83))
			e->addx(itofix(2));
    }
	else if(e->getx() > itofix(-166 + 83))
		e->subx(itofix(2));
	else
		e->deactivate();
	e->addy(e->internal[1]);
	if(e->gety() > itofix(240 + 120))
		e->suby(itofix(480));
	e->internal[1] -= (e->internal[1] > itofix(1)) * 8;
}

void cb_Pattern_2_rightDoor(cb_args)
{
    if(!e->internal[1])
		e->internal[1] = 128;
    if(Level::phase == PHASE_TRANSITION)
    {
		if(e->getx() > itofix(154 + 83))
			e->subx(itofix(2));		
    }
	else if(e->getx() < itofix(320 + 83))
		e->addx(itofix(2));
	else
		e->deactivate();
	e->addy(e->internal[1]);
	if(e->gety() > itofix(240 + 120))
		e->suby(itofix(480));
	e->internal[1] -= (e->internal[1] > itofix(1)) * 8;
}

// #########
// Chapter 2
// #########
void cb_Pattern_2_8(cb_args)
{
	e->internal[0] = fixtoi(itofix(256 * e->waveIndex) / 12) + Level::waveTimer / 2;
	e->internal[1] = Level::waveTimer < 15 * Constants::FPS ? 160 - min(Level::waveTimer * 2, 80) : 160 - 80 + (Level::waveTimer - 15 * Constants::FPS) / 2;
	e->setx(e->internal[1] * fixcos(e->internal[0]) + itofix(160));
	e->sety(e->internal[1] * fixsin(e->internal[0]) + itofix(120));
	e->rotationAngle++;
}

void cb_Pattern_2_9(cb_args)
{
	e->internal[0] = fixtoi(itofix(256 * e->waveIndex) / 12) - Level::waveTimer / 2;
	e->internal[1] = Level::waveTimer < 15 * Constants::FPS ? 200 - min(Level::waveTimer * 2, 80) : 200 - 80 + (Level::waveTimer - 15 * Constants::FPS) / 2;
	e->setx(e->internal[1] * fixcos(e->internal[0]) + itofix(160));
	e->sety(e->internal[1] * fixsin(e->internal[0]) + itofix(120));
	e->rotationAngle++;
}

void cb_Pattern_2_10(cb_args)
{
	if (!e->internal[0])
	{
		e->internal[1] = e->waveIndex * 21;
		e->internal[2] = 160;
		e->internal[0] = 1;
	}
	e->internal[1]++;
	e->internal[2]--;
	e->setx(e->internal[2] * fixcos(e->internal[1]) + itofix(160));
	e->sety(e->internal[2] * fixsin(e->internal[1]) + itofix(120));
	e->rotationAngle++;
}

void cb_Pattern_2_11(cb_args)
{
	if (!e->internal[0])
	{
		e->internal[1] = e->waveIndex * 21;
		e->internal[2] = 160;
		e->internal[0] = 1;
	}
	e->internal[1]--;
	e->internal[2]--;
	e->setx(e->internal[2] * fixcos(e->internal[1]) + itofix(160));
	e->sety(e->internal[2] * fixsin(e->internal[1]) + itofix(120));
	e->rotationAngle++;
}

void cb_Pattern_2_12(cb_args)
{
	if (!(Level::waveTimer % (40 * 2))) // boxes move by a pixel every frame ; let 2 boxes pass by
	{
		e->internal[0] ^= 1;
		Enemy *ce = Level::enemiesArray->add(e->getx(), e->gety(), 20,
											 static_cast<int>(e->internal[0] ? LUTs::BaseImageId::BOX_SHADOW_1 : LUTs::BaseImageId::BOX_SHADOW_1),
											 Pattern_box, 0, e->internal[0] ? Constants::SHADOW : Constants::LIGHT, 0, 5, false, TYPE_BREAKABLE_PROP);
		ce->internal[1] = 3; // move right
	}
	e->beAprop();
}

void cb_Pattern_2_13(cb_args)
{
	if (!(Level::waveTimer % (40 * 2)))
	{
		e->internal[0] ^= 1;
		Enemy *ce = Level::enemiesArray->add(e->getx(), e->gety(), 20,
											 static_cast<int>(e->internal[0] ? LUTs::BaseImageId::BOX_SHADOW_1 : LUTs::BaseImageId::BOX_SHADOW_1),
											 Pattern_box, 0, e->internal[0], 0, 5, false, TYPE_BREAKABLE_PROP);
		ce->internal[1] = 2; // move left
	}
	e->beAprop();
}

void cb_Pattern_2_14(cb_args)
{
	e->internal[0] = 1;
	e->internal[1] = -1;
	e->beAbox();
}

void cb_Pattern_2_15(cb_args)
{
	e->internal[0] = e->getx() < itofix(160) ? 2 : 4;
	e->internal[1] = 0;
	e->beAbox();
	if (!(Level::waveTimer % 16))
		Level::bArray->add(e->getx(), e->gety() + itofix(24), 64, itofix(1), static_cast<int>(LUTs::BaseImageId::ENEMY_BULLET_1_LIGHT), !e->polarity, true, e->getCamRel());
}

void cb_Pattern_2_16(cb_args)
{
	if (!(Level::waveTimer % (fixtoi(fixdiv(itofix(40), 256 - FCAMERA_SPEED * 2)))))
	{
		if (e->internal[0] % 2)
		{
			Enemy *ce = Level::enemiesArray->add(e->getx(), e->gety(), 1, static_cast<int>(LUTs::BaseImageId::BOX_SOLID_1), Pattern_box, 0, Constants::LIGHT, 0, 0, false, TYPE_PROP);
			ce->internal[1] = 1; // move down
		}
		else
		{
			Enemy *ce = Level::enemiesArray->add(e->getx(), e->gety(), 20,
												 static_cast<int>(e->internal[0] >> 1 ? LUTs::BaseImageId::BOX_SHADOW_1 : LUTs::BaseImageId::BOX_SHADOW_1),
												 Pattern_box, 0, e->internal[0] >> 1, 0, 5, false, TYPE_BREAKABLE_PROP);
			ce->internal[0] = 0x6; // fire left and right
			ce->internal[1] = 1; // move down
		}
		e->internal[0]++;
		e->internal[0] %= 4;
	}
	if (fToScreenY(e->gety(), e->getCamRel()) > 0) e->deactivate(); // stop spawning boxes when the spawnpoint is visible
}

void cb_Pattern_2_17(cb_args)
{
	e->addy(256);
	e->rotationAngle++;
}

void cb_Pattern_2_18(cb_args)
{
	e->setRotation(128);
	if (!(Level::waveTimer % 30))
		Level::enemiesArray->add(fToScreenX(e->getx(), e->getCamRel()), fToScreenY(e->gety(), e->getCamRel()), 1, static_cast<int>(LUTs::BaseImageId::ENEMY_SHIP_1_SHADOW), Pattern_2_20, 0, e->polarity, true, 2, false, TYPE_ENEMY);
	e->beAprop();
}

void cb_Pattern_2_19(cb_args)
{
	if (!((Level::waveTimer + 15) % 30))
		Level::enemiesArray->add(fToScreenX(e->getx(), e->getCamRel()), fToScreenY(e->gety(), e->getCamRel()), 1, static_cast<int>(LUTs::BaseImageId::ENEMY_SHIP_1_LIGHT), Pattern_2_20, 0, e->polarity, true, 2, false, TYPE_ENEMY);
	e->beAprop();
}

void cb_Pattern_2_20(cb_args)
{
	if (e->getx() < itofix(150))
	{
		e->rotationAngle = 64;
		e->addx(192);
		e->addy(FCAMERA_SPEED);
	}
	else if (e->getx() > itofix(170))
	{
		e->rotationAngle = 192;
		e->subx(192);
		e->addy(FCAMERA_SPEED);
	}
	else
	{
		e->rotationAngle = 0;
		e->addy(96 + FCAMERA_SPEED * 3);
	}
}

void cb_Pattern_2_21(cb_args)
{
	if (!(Level::waveTimer % 2))
	{
		if (e->getx() < itofix(160))
			e->rotationAngle--;
		else
			e->rotationAngle++;
	}
	if (!(Level::waveTimer % 6))
	{
		Rect temp;
		for (int i = 0; i < 4; i++)
		{
			rotate(itofix(24), 0, 0, 0, e->rotationAngle + i * 64, &temp);
			Level::bArray->add(e->getx() + temp.x, e->gety() + temp.y, -(e->rotationAngle + i * 64), 256, static_cast<int>(LUTs::BaseImageId::ENEMY_BULLET_0_LIGHT), i % 2 ? Constants::SHADOW : Constants::LIGHT, true, e->getCamRel());
		}
	}
	e->beAprop();
}

void cb_Pattern_2_22(cb_args)
{
	e->addy(FCAMERA_SPEED);
	e->rotationAngle++;
}

void cb_Pattern_2_23(cb_args)
{
	if (!e->internal[1])
	{
		e->internal[0] = -30;
		e->internal[1] = 1;
	}
	e->internal[0] += 2;
	e->setx((fixsin(Level::waveTimer) / 25 + 30) * fixcos(Level::waveTimer + (e->waveIndex % 6) * 42) + itofix(160));
	e->sety((fixsin(Level::waveTimer) / 25 + 30) * fixsin(Level::waveTimer +(e->waveIndex % 6) * 42) + e->internal[0] * FCAMERA_SPEED);
	e->rotationAngle++;
}

void cb_Pattern_2_24(cb_args)
{
	if (fixtoi(e->getx()) < 250 && !e->internal[0])
	{
		Level::bArray->fire_laser(e, e->polarity, 64);
		e->internal[0] = 1;
	}
	e->subx(128);
	if (fixtoi(e->getx()) < 70)
	{
		Level::bArray->stop_laser(e);
	}
}

void cb_Pattern_2_25(cb_args)
{
	if (!e->internal[0])
	{
		if (fixtoi(e->getx()) < 160 - 40) // center of the screen minus one box
			e->addx(192);
		else if (fixtoi(e->getx()) > 200) // center of the screen plus one box
			e->subx(192);
		else
		{
			Level::bArray->fire_laser(e, e->polarity, 64);
			e->internal[0] = 1;
		}
	}
	else if (e->internal[1] < INV_CAMERA_SPEED * 40 * 3) // keep firing until 3 boxes pass by
		e->internal[1]++;
	else
	{
		Level::bArray->stop_laser(e);
		e->addx(((e->getx() > itofix(160)) * 2 - 1) * 256);
	}
}

void cb_Pattern_2_26(cb_args)
{
	if (fixtoi(e->getx()) > 70 && !e->internal[0])
	{
		Level::bArray->fire_laser(e, e->polarity, 64);
		e->internal[0] = 1;
	}
	e->addx(128);
	if (fixtoi(e->getx()) > 250)
	{
		Level::bArray->stop_laser(e);
	}
}

void cb_Pattern_2_27(cb_args)
{
	e->internal[0] = 0x4;
	e->internal[2] = 0x4;
	e->beAbox();
}

void cb_Pattern_2_28(cb_args)
{
	e->internal[0] = 0x2;
	e->internal[2] = 0x2;
	e->beAbox();
}

void cb_Pattern_2_29(cb_args)
{
	if (!(Level::waveTimer % 2))
	{
		e->rotationAngle--;
	}
	if (!(Level::waveTimer % 6))
	{
		Rect temp;
		for (int i = 0; i < 4; i++)
		{
			rotate(itofix(24), 0, 0, 0, e->rotationAngle + i * 64, &temp);
			Level::bArray->add(e->getx() + temp.x, e->gety() + temp.y, -(e->rotationAngle + i * 64), 256, static_cast<int>(LUTs::BaseImageId::ENEMY_BULLET_0_LIGHT), e->polarity, true, e->getCamRel());
		}
	}
	e->beAprop();
}

void cb_Pattern_2_30(cb_args)
{
	e->setRotation(128);
	if (!(G_gpTimer % 45))
		Level::enemiesArray->add(fToScreenX(e->getx(), e->getCamRel()), fToScreenY(e->gety(), e->getCamRel()), 1, static_cast<int>(LUTs::BaseImageId::ENEMY_SHIP_1_LIGHT), Pattern_2_32, 0, e->polarity, true, 2, false, TYPE_ENEMY);
	e->beAprop();
}

void cb_Pattern_2_31(cb_args)
{
	if (!(G_gpTimer % 45))
		Level::enemiesArray->add(fToScreenX(e->getx(), e->getCamRel()), fToScreenY(e->gety(), e->getCamRel()), 1, static_cast<int>(LUTs::BaseImageId::ENEMY_SHIP_1_SHADOW), Pattern_2_33, 0, e->polarity, true, 2, false, TYPE_ENEMY);
	e->beAprop();
}

void cb_Pattern_2_32(cb_args)
{
	e->rotationAngle = -64;
	e->addx(256);
	e->addy(FCAMERA_SPEED);
}

void cb_Pattern_2_33(cb_args)
{
	e->rotationAngle = -64;
	e->subx(256);
	e->addy(FCAMERA_SPEED);
}

void cb_Pattern_2_34(cb_args)
{
	if (!e->internal[0])
	{
		e->internal[0] = 1;
		e->internal[1] = e->gety();
		e->rotationAngle = (e->waveIndex % 12) * 21 + (e->waveIndex > 11 ? 11 : 0);
		e->internal[2] = 100;
	}
	if(!(Level::waveTimer % 2)) e->rotationAngle += e->polarity == Constants::LIGHT ? -1 : 1;
	e->setx(fixcos(e->rotationAngle) * e->internal[2] + itofix(160));
	e->sety(max(fixsin(e->rotationAngle) * e->internal[2] + e->internal[1], itofix(-80)));
	if (fixtoi(fToScreenY(e->internal[1], e->getCamRel())) < 120)
		e->internal[1] += FCAMERA_SPEED;
	else if (e->internal[0] < 6 * Constants::FPS)
	{
		e->internal[0]++;
	}
	else e->internal[2]++;
}

void cb_Pattern_2_35(cb_args)
{
	if (!e->internal[0])
	{
		e->internal[0] = 1;
		e->rotationAngle = e->getx() < itofix(160) ? 32 : -32;
	}
	if (!e->internal[1])
	{
		if (e->getx() < itofix(120))
			e->addx(128);
		else if (e->getx() > itofix(200))
			e->subx(128);
		else
		{
			e->internal[1] = 1;
			Level::bArray->fire_laser(e, e->polarity, 64);
		}
	}
	else
	{
		if (e->internal[0] < 512)
			e->internal[0]++;
		else
		{
			Level::bArray->stop_laser(e);
			if (e->getx() < itofix(160))
				e->subx(128);
			else if (e->getx() > itofix(160))
				e->addx(128);
		}

		if (!(Level::waveTimer % 6))
		{
			Fixed angle = ~e->angleToEntity(Level::p) + 64;
			if (e->rotationAngle != angle)
			{
				if (((e->rotationAngle - angle) & 0xff) > 128)
					e->rotationAngle++;
				else
					e->rotationAngle--;
			}
		}
	}
}

void cb_Pattern_2_36(cb_args)
{
	if (!e->internal[0])
	{
		e->rotationAngle = (e->waveIndex % 2) * 128;
	}
	e->internal[0]++;
	e->rotationAngle++;
	e->setx(fixcos(e->rotationAngle) * 30);
	e->sety(fixsin(e->rotationAngle) * 30);
	int t = e->internal[0] % 100;
	if (!(t & 3) && t / 4 < 3)
		Level::bArray->add(e->getx(), e->gety(), 64, itofix(2), static_cast<int>(LUTs::BaseImageId::ENEMY_BULLET_1_LIGHT), e->polarity, true, e->getCamRel());
}

void cb_Pattern_2_wall(cb_args)
{
	if (fixtoi(fToScreenY(e->gety(), e->getCamRel())) >= 120)
	{
		e->addx( ((e->getx() > itofix(160)) * 2 - 1) * 48);
	}
}

void cb_Pattern_2_bossWeakPoint(cb_args)
{
	if(!(G_gpTimer % 8)) e->internal[0] = (e->internal[0] + 1) % 5;
	e->img = LUTs::bossImage((e->getPolarity() == Constants::LIGHT ? LUTs::BossImageId::BOSS2_HITPOINT_LEFT_1 : LUTs::BossImageId::BOSS2_HITPOINT_RIGHT_1), e->internal[0]);
}

void cb_Pattern_2_bossShield(cb_args)
{
	if (e->internal[0] == 1)
		e->suby((e->rawy() > 0) * 12);
	else if (e->internal[0] == 2)
	{
		if (e->isJointed)
		{
			e->setx(e->getx());
			e->sety(e->gety());
			e->isJointed = false;
		}
		e->addy(itofix(1));
		e->rotationAngle += 1;
	}
}

enemy_callback enemyCallbacks[] = { cb_Pattern_null, cb_Pattern_box, cb_Pattern_prop, cb_Pattern_1_1, cb_Pattern_1_2, cb_Pattern_1_3, cb_Pattern_1_4, cb_Pattern_1_5, cb_Pattern_1_6,
	cb_Pattern_1_7, cb_Pattern_1_8, cb_Pattern_1_9, cb_Pattern_1_10, cb_Pattern_1_11, cb_Pattern_1_12, cb_Pattern_1_13, cb_Pattern_1_14, cb_Pattern_1_15, cb_Pattern_1_16,
	cb_Pattern_1_17, cb_Pattern_1_18, cb_Pattern_1_19, cb_Pattern_1_20, cb_Pattern_1_21, cb_Pattern_1_boss, cb_Pattern_1_bossGrenade,
	cb_Pattern_2_1, cb_Pattern_2_2, cb_Pattern_2_3, cb_Pattern_2_4, cb_Pattern_2_5, cb_Pattern_2_6, cb_Pattern_2_7, cb_Pattern_2_leftDoor, cb_Pattern_2_rightDoor,
	cb_Pattern_2_8, cb_Pattern_2_9, cb_Pattern_2_10, cb_Pattern_2_11, cb_Pattern_2_12, cb_Pattern_2_13, cb_Pattern_2_14, cb_Pattern_2_15, cb_Pattern_2_16, cb_Pattern_2_17,
	cb_Pattern_2_18, cb_Pattern_2_19, cb_Pattern_2_20, cb_Pattern_2_21, cb_Pattern_2_22, cb_Pattern_2_23, cb_Pattern_2_24, cb_Pattern_2_25, cb_Pattern_2_26, cb_Pattern_2_27,
	cb_Pattern_2_28, cb_Pattern_2_29, cb_Pattern_2_30, cb_Pattern_2_31, cb_Pattern_2_32, cb_Pattern_2_33, cb_Pattern_2_34, cb_Pattern_2_35, cb_Pattern_2_36, cb_Pattern_2_wall,
	cb_Pattern_2_bossWeakPoint, cb_Pattern_2_bossShield
};
