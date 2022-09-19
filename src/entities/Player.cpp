#include "entities/Player.hpp"

#include "GameSystems.hpp"
#include "graphics/Particles.hpp"
#include "handlers/DrawingCandidates.hpp"
#include "handlers/SoundHandler.hpp"
#include "helpers/Constants.hpp"
#include "level/Level.hpp"
#include "n2DLib/n2DLib_math.h"
#include "utils.hpp"

Player::Player() : Entity()
{
	img[LUTs::PlayerAnimId::LIGHT] = LUTs::baseImage(LUTs::BaseImageId::PLAYER_SHIP_LIGHT);
	img[LUTs::PlayerAnimId::SHADOW] = LUTs::baseImage(LUTs::BaseImageId::PLAYER_SHIP_SHADOW);
	img[LUTs::PlayerAnimId::LIGHT + LUTs::PlayerAnimId::SWITCHING0] = LUTs::baseImage(LUTs::BaseImageId::PLAYER_SHIP_POLARITYSWITCH_0_LIGHT);
	img[LUTs::PlayerAnimId::SHADOW + LUTs::PlayerAnimId::SWITCHING0] = LUTs::baseImage(LUTs::BaseImageId::PLAYER_SHIP_POLARITYSWITCH_0_SHADOW);
	img[LUTs::PlayerAnimId::LIGHT + LUTs::PlayerAnimId::SWITCHING1] = LUTs::baseImage(LUTs::BaseImageId::PLAYER_SHIP_POLARITYSWITCH_1_LIGHT);
	img[LUTs::PlayerAnimId::SHADOW + LUTs::PlayerAnimId::SWITCHING1] = LUTs::baseImage(LUTs::BaseImageId::PLAYER_SHIP_POLARITYSWITCH_1_SHADOW);
	deathCounter = 0;
	x = 0;
	y = 0;
	
	Entity::activate();

	reset();
}

void Player::reset()
{
	polarity = Constants::LIGHT;
	fireRepeat = false;
	polarityRepeat = false;
	isSwitchingPolarity = 0;
	fireDelay = 12;
	lives = 4;
}

void Player::handle(KeyEvent kEv)
{
	Rect r, temp;
	
	if(active)
	{
		// Display the player
		r.x = fixtoi(x);
		r.y = fixtoi(y);
		
		GS->DC->add(img[((isSwitchingPolarity / 4) * 2) + polarity], &r, false, Constants::CamRelation::DEFAULT);
		
		if(deathCounter)
		{
			GS->DC->add(LUTs::baseImage(LUTs::BaseImageId::PLAYER_SHIP_INVINCIBLE_LIGHT, polarity), &r, false, Constants::CamRelation::DEFAULT);
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
			
		x = r.x < GS->minX ? itofix(GS->minX + temp.x) : (r.x > GS->maxX - (temp.x * 2) ? itofix(GS->maxX - temp.x) : x);
		y = r.y < 0 ? itofix(temp.y) : (r.y > 240 - (temp.y * 2) ? itofix(240 - temp.y) : y);
		
		if(KPOLARITY(kEv))
		{
			if (!polarityRepeat)
			{
				switchPolarity();
				GS->soundSystem->quickPlaySFX(LUTs::sound(LUTs::SoundId::PLAYER_SWITCH));
			}
			polarityRepeat = true;
		}
		else
			polarityRepeat = false;
		
		if(KFRAGMENT(kEv))
		{
			if(GS->power > 9)
			{
				GS->soundSystem->quickPlaySFX(LUTs::sound(LUTs::SoundId::BULLET_FIRE_FRAGMENT));
				GS->hasFiredOnce = true;
				for(int i = 0; i < GS->power / 10; i++)
					Level::bArray->add_fragment(x, y, 64 + ((i % 2) ? 10 + (i / 2) * 21 : -10 - (i / 2) * 21), this, polarity, false);
				GS->power = 0;
			}
		}
		
		if(KFIRE(kEv))
		{
			if(!fireDelay)
			{
				GS->hasFiredOnce = true;
				if(fireRepeat)
				{
					// fire 2 bullets if the key is being held
					Level::bArray->add(x - itofix(img[0][0]) / 4, y, 192, itofix(6), LUTs::BaseImageId::PLAYER_BULLET_LIGHT, polarity, false, Constants::CamRelation::DEFAULT);
					Level::bArray->add(x + itofix(img[0][0]) / 4, y, 192, itofix(6), LUTs::BaseImageId::PLAYER_BULLET_LIGHT, polarity, false, Constants::CamRelation::DEFAULT);
					fireDelay = 4;
					int x1 = x - itofix(img[0][0]) / 4;
					int x2 = x + itofix(img[0][0]) / 4;
					for(int i = 0; i < 8; i++)
					{
						GS->particles->add(x1, y, 192 + (rand() % 32) - 16, itofix(2), polarity, 8);
						GS->particles->add(x2, y, 192 + (rand() % 32) - 16, itofix(2), polarity, 8);
					}
				}
				else
				{
					// fire 1 bullet
					Level::bArray->add(x, y, 192, itofix(6), LUTs::BaseImageId::PLAYER_BULLET_LIGHT, polarity, false, Constants::CamRelation::DEFAULT);
					fireDelay = 8;
					fireRepeat = true;
					for(int i = 0; i < 8; i++)
						GS->particles->add(x, y, 192 + (rand() % 32) - 16, itofix(2), polarity, 8);
				}
				GS->soundSystem->quickPlaySFX(LUTs::sound(LUTs::SoundId::BULLET_FIRE_PLAYER_BULLET));
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
			GS->DC->add(LUTs::baseImage(LUTs::BaseImageId::PLAYER_EXPLOSION_0, deathCounter), &r, false, Constants::CamRelation::DEFAULT);
			// Death animation
			// Uses frameskipping as a counter
			//if(!(GS->skipFrame % 8))
			if(!(GS->chapterTimer % 4))
				deathCounter++;
		}
		else if(deathCounter == 12)
		{
			polarity = Constants::LIGHT;
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
			GS->DC->add(LUTs::baseImage(LUTs::BaseImageId::PLAYER_SHIP_LIGHT), &r, false, Constants::CamRelation::DEFAULT);
			y -= itofix(2);
		}
		else active = true;
	}
}

bool Player::getPolarity() const
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
	GS->chainStatus = 0;
	GS->inChainCount = 0;
	GS->power = 0;
	GS->soundSystem->quickPlaySFX(LUTs::sound(LUTs::SoundId::PLAYER_DEATH));
}

int Player::getLives() const
{
	return lives;
}

void Player::setLives(int l)
{
	lives = l;
}

bool Player::isHurtable() const
{
	return active && !deathCounter;
}