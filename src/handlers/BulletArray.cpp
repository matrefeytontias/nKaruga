#include "common.h"

BulletArray::BulletArray()
{
	clear();
	// lasers too
	for(int i = 0; i < MAX_LASER; i++)
		data_laser[i].deactivate();
	laserCount = 0;
}

BulletArray::~BulletArray()
{
}

void BulletArray::handle()
{
	bool destroyBullet;
	int bossDamaged;
	
	// Bullets
	for (int i = 0; i < bulletCount; i++)
	{
		Bullet *cb = &data[i];
		destroyBullet = false;
		if(cb->isActive())
		{
			if(cb->hurtsPlayer() && Level::p->isHurtable())
			{
				// Check collisions with player if he's not dead already
				if(cb->getPolarity() != Level::p->getPolarity())
				{
					// the player has a 1px hitbox (for now) (but that actually seems to Level::be enough)
					if(Level::p->getx() >= cb->getx() - itofix(cb->img[0] / 2) && Level::p->getx() < cb->getx() + itofix(cb->img[0] / 2)
					&& Level::p->gety() >= cb->gety() - itofix(cb->img[1] / 2) && Level::p->gety() < cb->gety() + itofix(cb->img[1] / 2))
					{
						Level::p->hurt();
						destroyBullet = true;
					}
				}
				else
				{
					if(sq(fixtoi(cb->getx() - Level::p->getx())) + sq(fixtoi(cb->gety() - Level::p->gety())) < sq(19)) // sqrt(player.w/2 ^2 + player.h/2 ^2)
					{
						destroyBullet = true;
						G_score += SCORE_ABSORB;
						G_power += G_power < MAX_POWER;
					}
				}
			}
			else if(!cb->hurtsPlayer())
			{
				// Check collisions with enemies (there are many more enemies than players)
				for(int j = 0; j < MAX_ENEMY; j++)
				{
					Enemy *ce = &Level::enemiesArray->data[j];
					if(ce->isActive() && !ce->isGhost())
					{
						if (ce->collide(cb->getx(), cb->gety()))
						{
							// Send a couple particles
							/*
							for(int k = 0; k < 8; k++)
								G_particles->add();
							*/
							if(ce->damage(cb->getPolarity(), 1))
								G_score += cb->getPolarity() != ce->getPolarity() ? SCORE_HIT_OP : SCORE_HIT;
							destroyBullet = true;
							// The same bullet can destroy several enemies if it hits them *during the same frame* !
						}
					}
				}
				
				// and possibly the boss
				if(Level::fightingBoss && Level::be->isHurtable())
				{
					bossDamaged = (Level::be->collisionCallbacks[Level::be->currentPattern])(Level::be, cb, 1);
					if(bossDamaged)
					{
						destroyBullet = true;
						if(bossDamaged > 0) Level::be->damage(bossDamaged);
					}
				}
			}

			// Check collisions with props
			// Props can rotate and shoot bullets at the same time, which can cause problems with AABBs
			// See utils.cpp for collideProp
			for (int j = 0; j < MAX_ENEMY; j++)
			{
				Enemy *ce = &Level::enemiesArray->data[j];
				if (ce->isActive() && ce->isProp())
				{
					if (ce->collide(cb->getx(), cb->gety()))
						destroyBullet = true;
				}
			}

			if(destroyBullet)
				deactivate(i, true);
			else
			{
				if(cb->handle())
					deactivate(i, false);
				else
					cb->draw();
			}
		}
		else break;
	}	
	
	// Power fragments
	for(int i = 0; i < fragmentCount; i++)
	{
		PowerFragment *cf = &data_fragment[i];
		destroyBullet = false;
		if(cf->isActive())
		{
			if(cf->hurtsPlayer() && Level::p->isHurtable())
			{
				// Check collisions with player
				if(cf->getPolarity() != Level::p->getPolarity())
				{
					// the player has a 1px hitbox (for now) (but that actually seems to Level::be enough)
					// power fragments have a 8*8 hitbox
					if(Level::p->getx() >= cf->getx() - itofix(4) && Level::p->getx() < cf->getx() + itofix(4)
					&& Level::p->gety() >= cf->gety() - itofix(4) && Level::p->gety() < cf->gety() + itofix(4))
					{
						Level::p->hurt();
						destroyBullet = true;
					}
				}
				else
				{
					if(sq(fixtoi(cf->getx() - Level::p->getx())) + sq(fixtoi(cf->gety() - Level::p->gety())) < sq(Level::p->img[0][0] / 2))
					{
						destroyBullet = true;
						G_score += SCORE_ABSORB * 10;
						G_power = min(G_power + 10, MAX_POWER);
					}
				}
			}
			else
			{
				// Check collisions with enemies
				// A power fragment can hit other enemies than its registered target
				if(cf->targetE)
				{
					for (int i = 0; i < MAX_ENEMY; i++)
					{
						Enemy *ce = &Level::enemiesArray->data[i];
						if(ce->isActive() && (ce->isDamageable() || !ce->isProp()))
						{
							if(cf->getx() - itofix(4) <= fToScreenX(ce->getx(), ce->getCamRel()) + itofix(ce->img[0] / 2) &&
							cf->getx() + itofix(4) >= fToScreenX(ce->getx(), ce->getCamRel()) - itofix(ce->img[0] / 2) &&
							cf->gety() - itofix(4) <= fToScreenY(ce->gety(), ce->getCamRel()) + itofix(ce->img[1] / 2) &&
							cf->gety() + itofix(4) >= fToScreenY(ce->gety(), ce->getCamRel()) - itofix(ce->img[1] / 2))
							{
								if(ce->damage(cf->getPolarity(), 10))
									G_score += cf->getPolarity() != ce->getPolarity() ? SCORE_HIT_OP : SCORE_HIT;
								destroyBullet = true;
							}
						}
					}
				}
				
				// and the boss, even
				if(Level::fightingBoss && Level::be->isHurtable())
				{
					// Create a placeholder bullet to pass to the collision callback
					Bullet temp;
					temp.activate(cf->getx(), cf->gety(), 0, 0, 0, cf->getPolarity(), false, CAMREL_NONE);
					bossDamaged = (Level::be->collisionCallbacks[Level::be->currentPattern])(Level::be, &temp, 10);
					temp.deactivate();
					if(bossDamaged)
					{
						destroyBullet = true;
						Level::be->damage(bossDamaged);
					}
				}
			}
			if(destroyBullet)
				deactivate_fragment(i);
			else
			{
				if(cf->handle())
					deactivate_fragment(i);
				else
					cf->draw();
			}
		}
		else break;
	}
	
	// Homings
	for(int i = 0; i < homingCount; i++)
	{
		Homing* ch = &data_homing[i];
		destroyBullet = false;
		if(ch->isActive())
		{
			if(Level::p->isHurtable())
			{
				// Check collisions with player
				if(ch->getPolarity() != Level::p->getPolarity())
				{
					// the player has a 1px hitbox (for now) (but that actually seems to Level::be enough)
					// homings have a 16*16 hitbox
					if(Level::p->getx() >= ch->getx() - itofix(8) && Level::p->getx() < ch->getx() + itofix(8)
					&& Level::p->gety() >= ch->gety() - itofix(8) && Level::p->gety() < ch->gety() + itofix(8))
					{
						Level::p->hurt();
						destroyBullet = true;
					}
				}
				else
				{
					if(sq(fixtoi(ch->getx() - Level::p->getx())) + sq(fixtoi(ch->gety() - Level::p->gety())) < sq(Level::p->img[0][0] / 2))
					{
						destroyBullet = true;
						G_score += SCORE_ABSORB * 5;
						G_power = min(G_power + 5, MAX_POWER);
					}
				}
			}
			if(destroyBullet)
				deactivate_homing(i);
			else
			{
				if(ch->handle())
					deactivate_homing(i);
				else
					ch->draw();
			}
		}
		else break;
	}
	
	// Lasers
	for(int i = 0; i < MAX_LASER; i++)
	{
		Rect *r, r1, r2;
		Laser *cl = &data_laser[i];
		if(cl->isActive())
		{
			if(cl->origin->isActive())
			{
				r = cl->getVector();
				cl->getSides(&r1, &r2);
				
				if(Level::p->isHurtable())
				{
					// Uses cartesian hitbox for checking collision with player
					// First, see if the player is behind or in front of the origin of the laser using the dot product
					// a . b = ||a|| ||b|| cos(angle(a, b)) ; if a . b < 0 then |angle| > 90�
					if ((fixtoi(Level::p->getx()) - r->x) * r->w + (fixtoi(Level::p->gety()) - r->y) * r->h > 0)
					{
						// Then, see if the player is not too far
						if(sq(fixtoi(Level::p->getx()) - r->x) + sq(fixtoi(Level::p->gety()) - r->y) <= sq(cl->getAmplitude()))
						{
							// if we're not too far, carry on collision checking
							// calculate the laser's cartesian equation and apply it to each of its sides
							// ax + by + c = 0
							int a, b, c1, c2;
							a = r->h;
							b = -r->w;
							c1 = -(a * r1.x + b * r1.y);
							c2 = -(a * r2.x + b * r2.y);
						
							if(Level::p->getPolarity() != cl->getPolarity())
							{
								int temp = a * fixtoi(Level::p->getx()) + b * fixtoi(Level::p->gety());
								// Work the player's 1 px hitbox
								if(sign(temp + c1) != sign(temp + c2))
									// Hit !
									Level::p->hurt();
							}
							else
							{
								int temp1 = a * (fixtoi(Level::p->getx()) - Level::p->img[0][0] / 2) + b * fixtoi(Level::p->gety());
								int temp2 = a * (fixtoi(Level::p->getx()) + Level::p->img[0][0] / 2) + b * fixtoi(Level::p->gety());
							
								if(sign(temp1 + c1) != sign(temp1 + c2) || sign(temp2 + c1) != sign(temp2 + c2))
								{
									// Hit, but doesn't hurt
									cl->setAmplitude((int)sqrt((float)sq(fixtoi(Level::p->getx()) - r->x) + sq(fixtoi(Level::p->gety()) - r->y)));
									// Using G_gpTimer as a delay
									if (!(G_gpTimer % 2))
									{
										G_power += G_power < MAX_POWER;
										G_score += SCORE_ABSORB;
									}
									// Lasers are powerful, so they push the player
									Level::p->setx(Level::p->getx() + fixcos(cl->angle) / 2);
									Level::p->sety(Level::p->gety() + fixsin(cl->angle) / 2);
								
									// Add particles
									int k = (rand() % 4) + 1;
									for(int j = 0; j < k; j++)
									{
										Fixed a = cl->angle + 128 + (rand() % 64) - 32;
										G_particles->add(Level::p->getx(), Level::p->gety(), a, itofix(1), cl->getPolarity(), 32);
									}
								}
							}
						}
					}
				}
				
				cl->handle();
				// Lasers are very complicated to draw ; only do it if necessary
				//if(!(G_skipFrame % 4))
					cl->draw();
			}
			else
				cl->deactivate();
		}
	}
}

// TODO : Add some particles each time a bullet is fired

void BulletArray::add(Fixed _x, Fixed _y, Fixed a, Fixed r, int imgID, bool _p, bool _h, int camRel)
{
	if(bulletCount < MAX_BULLET)
	{
		data[bulletCount].activate(_x, _y, a, r, imgID, _p, _h, camRel);
		bulletCount++;
		// Only handle sound for on-screen enemy bullets
		if (_h && _x >= 0 && _x <= itofix(320) && _y >= 0 && _y <= itofix(240))
		{
			int d = (imgID - image_LUT_enemy_bullet_0_light) / 2;
			Level::soundSystem->quickPlaySFX(sound_entries[SD_BULLET_FIRE_ENEMY_0 + d]);
		}
	}
}

void BulletArray::add_fragment(Fixed _x, Fixed _y, Fixed angle, Player* targetP, bool _p, bool _h)
{
	if(fragmentCount < MAX_FRAGMENT)
	{
		data_fragment[fragmentCount].activate(_x, _y, angle, targetP, _p, _h);
		fragmentCount++;
	}
}

void BulletArray::add_homing(Fixed _x, Fixed _y, Fixed angle, Player* target, bool _p)
{
	if(homingCount < MAX_HOMING)
	{
		data_homing[homingCount].activate(_x, _y, angle, target, _p);
		homingCount++;
		Level::soundSystem->quickPlaySFX(sound_entries[SD_BULLET_FIRE_ENEMY_HOMING]);
	}
}

void BulletArray::fire_laser(Enemy *e, bool _p, Fixed _a)
{
	data_laser[laserCount].activate(e, _p, _a);
	laserCount = (laserCount + 1) % MAX_LASER;
}

void BulletArray::deactivate(int n, bool playSound)
{
	bulletCount--;
	data[n].deactivate();
	for(int i = n; i < bulletCount; i++)
		data[i] = data[i + 1];
	if(playSound) Level::soundSystem->quickPlaySFX(sound_entries[SD_BULLET_IMPACT]);
	//data[bulletCount].deactivate();
}

void BulletArray::deactivate_fragment(int n)
{
	fragmentCount--;
	data_fragment[n].deactivate();
	for(int i = n; i < fragmentCount; i++)
		data_fragment[i] = data_fragment[i + 1];
	//data_fragment[fragmentCount].deactivate();
}

void BulletArray::deactivate_homing(int n)
{
	homingCount--;
	data_homing[n].deactivate();
	for(int i = n; i < homingCount; i++)
		data_homing[i] = data_homing[i + 1];
	//data_homing[homingCount].deactivate();
}

void BulletArray::stop_laser(Enemy *e)
{
	for(int i = 0; i < MAX_LASER; i++)
	{
		if(data_laser[i].origin == e)
		{
			data_laser[i].deactivate();
			break;
		}
	}
}

// Destroy all bullets except for lasers
void BulletArray::clear()
{
	for(int i = 0; i < bulletCount; i++)
		data[i].deactivate();
	for(int i = 0; i < fragmentCount; i++)
		data_fragment[i].deactivate();
	for(int i = 0; i < homingCount; i++)
		data_homing[i].deactivate();
	bulletCount = 0;
	fragmentCount = 0;
	homingCount = 0;
}