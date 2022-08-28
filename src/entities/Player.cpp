#include "common.h"

Player::Player() : Entity()
{
	img[LIGHT] = image_entries[image_LUT_player_ship_light];
	img[SHADOW] = image_entries[image_LUT_player_ship_shadow];
	img[LIGHT + SWITCHING0] = image_entries[image_LUT_player_ship_polarityswitch_0_light];
	img[SHADOW + SWITCHING0] = image_entries[image_LUT_player_ship_polarityswitch_0_shadow];
	img[LIGHT + SWITCHING1] = image_entries[image_LUT_player_ship_polarityswitch_1_light];
	img[SHADOW + SWITCHING1] = image_entries[image_LUT_player_ship_polarityswitch_1_shadow];
	deathCounter = 0;
	active = true;
	x = 0;
	y = 0;
	reset();
}

void Player::reset()
{
	polarity = LIGHT;
	fireRepeat = false;
	polarityRepeat = false;
	isSwitchingPolarity = 0;
	fireDelay = 12;
	lives = 4;
}

Player::~Player()
{
}

void Player::handle(KeyEvent kEv)
{
	static Rect r, temp;
	
	if(active)
	{
		// Display the player
		r.x = fixtoi(x);
		r.y = fixtoi(y);
		
		DC->add(img[((isSwitchingPolarity / 4) * 2) + polarity], &r, false, CAMREL_NONE);
		
		if(deathCounter)
		{
			DC->add(image_entries[image_LUT_player_ship_invincible_light + polarity], &r, false, CAMREL_NONE);
			deathCounter--;
		}
		
		if(isSwitchingPolarity)
		{
			isSwitchingPolarity++;
		}
		
		if((isSwitchingPolarity / 4) == 3)
		{
			polarity = !polarity;
			isSwitchingPolarity = 0;
		}
		
		// And then only, player input
		if(KDOWN(kEv)) y += itofix(2);
		if(KLEFT(kEv)) x -= itofix(2);
		if(KRIGHT(kEv)) x += itofix(2);
		if(KUP(kEv)) y -= itofix(2);
			
		r.x = fixtoi(x) - (img[(isSwitchingPolarity / 8) * 2][0] / 2);
		r.y = fixtoi(y) - (img[(isSwitchingPolarity / 8) * 2][1] / 2);
			
		temp.x = img[(isSwitchingPolarity / 8) * 2][0] / 2;
		temp.y = img[(isSwitchingPolarity / 8) * 2][1] / 2;
			
		x = r.x < G_minX ? itofix(G_minX + temp.x) : (r.x > G_maxX - (temp.x * 2) ? itofix(G_maxX - temp.x) : x);
		y = r.y < 0 ? itofix(temp.y) : (r.y > 240 - (temp.y * 2) ? itofix(240 - temp.y) : y);
		
		if(KPOLARITY(kEv))
		{
			if (!polarityRepeat)
			{
				switchPolarity();
				Level::soundSystem->quickPlaySFX(sound_entries[SD_PLAYER_SWITCH]);
			}
			polarityRepeat = true;
		}
		else
			polarityRepeat = false;
		
		if(KPOWER(kEv))
		{
			if(G_power > 9)
			{
				Level::soundSystem->quickPlaySFX(sound_entries[SD_BULLET_FIRE_FRAGMENT]);
				G_hasFiredOnce = true;
				for(int i = 0; i < G_power / 10; i++)
					Level::bArray->add_fragment(x, y, 64 + ((i % 2) ? 10 + (i / 2) * 21 : -10 - (i / 2) * 21), this, polarity, false);
				G_power = 0;
			}
		}
		
		if(KFIRE(kEv))
		{
			if(!fireDelay)
			{
				G_hasFiredOnce = true;
				if(fireRepeat)
				{
					// fire 2 bullets if the key is being held
					Level::bArray->add(x - itofix(img[0][0]) / 4, y, 192, itofix(6), image_LUT_player_bullet_light, polarity, false, CAMREL_NONE);
					Level::bArray->add(x + itofix(img[0][0]) / 4, y, 192, itofix(6), image_LUT_player_bullet_light, polarity, false, CAMREL_NONE);
					fireDelay = 4;
					int x1 = x - itofix(img[0][0]) / 4;
					int x2 = x + itofix(img[0][0]) / 4;
					for(int i = 0; i < 8; i++)
					{
						G_particles->add(x1, y, 192 + (rand() % 32) - 16, itofix(2), polarity, 8);
						G_particles->add(x2, y, 192 + (rand() % 32) - 16, itofix(2), polarity, 8);
					}
				}
				else
				{
					// fire 1 bullet
					Level::bArray->add(x, y, 192, itofix(6), image_LUT_player_bullet_light, polarity, false, CAMREL_NONE);
					fireDelay = 8;
					fireRepeat = true;
					for(int i = 0; i < 8; i++)
						G_particles->add(x, y, 192 + (rand() % 32) - 16, itofix(2), polarity, 8);
				}
				Level::soundSystem->quickPlaySFX(sound_entries[SD_BULLET_FIRE_PLAYER_BULLET]);
			}
		}
		else
		{
			fireRepeat = false;
			fireDelay = 0;
		}
		
		if(fireDelay)
			fireDelay--;
	}
	else
	{
		if(deathCounter < 12)
		{
			r.x = fixtoi(x);
			r.y = fixtoi(y);
			DC->add(image_entries[image_LUT_player_explosion_0 + deathCounter], &r, false, CAMREL_NONE);
			// Death animation
			// Uses frameskipping as a counter
			//if(!(G_skipFrame % 8))
			if(!(G_gpTimer % 4))
				deathCounter++;
		}
		else if(deathCounter == 12)
		{
			polarity = LIGHT;
			x = itofix(160);
			y = itofix(260);
			deathCounter++;
		}
		// wait some frames so the player can breath
		else if(deathCounter < 128) deathCounter++;
		// get the player back in the game !
		else if(y > itofix(180))
		{
			r.x = fixtoi(x);
			r.y = fixtoi(y);
			DC->add(image_entries[image_LUT_player_ship_light], &r, false, CAMREL_NONE);
			y -= itofix(2);
		}
		else active = true;
	}
}

bool Player::getPolarity()
{
	return polarity;
}

void Player::switchPolarity()
{
	isSwitchingPolarity = 2;
}

void Player::hurt()
{
	lives--;
	active = false;
	deathCounter = 0;
	G_chainStatus = 0;
	G_frameChainOffset = 0;
	G_inChainCount = 0;
	G_power = 0;
	Level::soundSystem->quickPlaySFX(sound_entries[SD_PLAYER_DEATH]);
}

int Player::getLives()
{
	return lives;
}

void Player::setLives(int l)
{
	lives = l;
}

bool Player::isHurtable()
{
	return active && !deathCounter;
}