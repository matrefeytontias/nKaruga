#include "common.h"
#include "ExplosionEffect.h"
#include "levels.h" 
#include "gfx/kanji.h"
#include "misc_data.h"

#include <stack>

#define ENEMY_W(i) Level::enemiesArray->data[i].img[0]
#define ENEMY_H(i) Level::enemiesArray->data[i].img[1]

int G_gpTimer;
int G_minX = 0, G_maxX = 320;
int G_skipFrame = 0;
int G_killedThisFrame[MAX_ENEMY] = { -1 }, G_frameChainOffset = 0, G_chainStatus = 0, G_inChainCount = 0, G_maxChain = 0;
int G_score = 0, G_power = 0;
bool G_displayBg = true, G_fireback = true, G_hardMode = false;
bool G_hasFiredOnce = false;
bool G_runBoss = false;
int G_bossIntroChannel = -2;
int G_difficulty = 1;
bool G_usingArrows = false;

t_key G_downKey, G_leftKey, G_rightKey, G_upKey, G_fireKey, G_polarityKey, G_fragmentKey, G_pauseKey;

DrawingCandidates *DC;
Particles *G_particles;

void playGame();

static unsigned short image_cursor[] = { 5, 8, 1,
0, 0, 1, 1, 1,
1, 0, 0, 1, 1,
1, 1, 0, 0, 1,
1, 1, 1, 0, 0,
1, 1, 1, 0, 0,
1, 1, 0, 0, 1,
1, 0, 0, 1, 1,
0, 0, 1, 1, 1
};

inline void writeKeyToConfig(FILE* out, t_key* key)
{
	fputc(*key, out);
}

inline void readKeyFromConfig(FILE* in, t_key* key)
{
	*key = fgetc(in);
}

inline void writeToConfig(FILE* out)
{
	writeKeyToConfig(out, &G_fireKey);
	writeKeyToConfig(out, &G_polarityKey);
	writeKeyToConfig(out, &G_fragmentKey);
	writeKeyToConfig(out, &G_pauseKey);
	fputc(G_difficulty, out);
	fputc(G_usingArrows, out);
	fputc(G_displayBg, out);
}

inline void readFromConfig(FILE* in)
{
	readKeyFromConfig(in, &G_fireKey);
	readKeyFromConfig(in, &G_polarityKey);
	readKeyFromConfig(in, &G_fragmentKey);
	readKeyFromConfig(in, &G_pauseKey);
	G_difficulty = fgetc(in);
	G_usingArrows = !!fgetc(in);
	if (G_usingArrows)
	{
		G_downKey = SDL_SCANCODE_DOWN;
		G_leftKey = SDL_SCANCODE_LEFT;
		G_rightKey = SDL_SCANCODE_RIGHT;
		G_upKey = SDL_SCANCODE_UP;
	}
	else
	{
		G_downKey = SDL_SCANCODE_S;
		G_leftKey = SDL_SCANCODE_A;
		G_rightKey = SDL_SCANCODE_D;
		G_upKey = SDL_SCANCODE_W;
	}
	G_displayBg = !!fgetc(in);
}

int main(int argc, char **argv)
{
	UNUSED(argc);
	UNUSED(argv);

	int blink = 0;
	bool donePlaying = false, openedMenu = false;
	G_usingArrows = false;
	FILE* configFile;
	// Custom keys vars
	t_key* customKeys[KEYS_TO_BIND] = { &G_fireKey, &G_polarityKey, &G_fragmentKey, &G_pauseKey };
	int choice = 0;
	
	configFile = fopen(string_nKaruga_config, "rb");
	if(configFile)
	{
		readFromConfig(configFile);
		fclose(configFile);
	}
	else
	{
		G_fireKey = SDL_SCANCODE_I;
		G_polarityKey = SDL_SCANCODE_O;
		G_fragmentKey = SDL_SCANCODE_P;
		G_pauseKey = SDL_SCANCODE_M;
		G_downKey = SDL_SCANCODE_S;
		G_leftKey = SDL_SCANCODE_A;
		G_rightKey = SDL_SCANCODE_D;
		G_upKey = SDL_SCANCODE_W;
	}
	
	Level::init(1);
	
	G_particles = new Particles;
	DC = new DrawingCandidates;

	printf("Bleh 5");
	// Mix_Volume(-1, 0);
	// Mix_VolumeMusic(0);
	
	printf("Building game LUTs ...\n");
	buildGameLUTs();
	printf("Done\n");
	
	// Init things
	initBuffering();
	clearBufferW();
	initExplosionEngine();
	timer_load(1, 0);
	
	Level::init(1);

	while(!donePlaying)
	{
		const Uint8 *keys = SDL_GetKeyboardState(NULL);

		drawSprite(image_entries[image_LUT_titleScreen], 0, 0, 0, 0);
		if(!openedMenu)
		{
			int x = (320 - strlen(string_title) * 8) / 2;
			int y = 160;
			if(blink % 1024 < 512)
				drawString(&x, &y, x, string_title, 0, 0xffff);
			blink++;
			if(isKeyPressed(SDL_SCANCODE_RETURN))
			{
				wait_no_key_pressed(SDL_SCANCODE_RETURN);
				openedMenu = true;
			}
		}
		else if(openedMenu)
		{
			void* v[] = { NULL, &G_difficulty, &G_usingArrows, NULL };
			MenuItem items[TITLE_OPTIONS];
			for (int i = 0; i < TITLE_OPTIONS; i++)
			{
				items[i].name = string_options[i];
				items[i].value = v[i];
				items[i].labels = NULL;
				items[i].labelsNb = 0;
			}
			items[1].labels = string_difficulties;
			items[1].labelsNb = 3;
			items[2].labels = string_bools;
			items[2].labelsNb = 2;
			Menu m(4, items[0], items[1], items[2], items[3]);
			choice = m.run();
			if (choice == -1) donePlaying = true;
			else if (choice == 3)
			{
				wait_no_key_pressed(SDL_SCANCODE_RETURN);
				clearBufferB();
				int x = 0, y = 0;
				drawString(&x, &y, 0, "Press the key you want to bind to this\naction.\n\n", 0xffff, 0);

				for (int i = 0; i < KEYS_TO_BIND; i++)
				{
					drawString(&x, &y, 0, string_keys[i], 0xffff, 0);
					updateScreen();
					while (!get_key_pressed(customKeys[i])) updateKeys();
					wait_no_key_pressed(*(customKeys[i]));
				}
				configFile = fopen(string_nKaruga_config, "wb");
				if (configFile)
				{
					writeToConfig(configFile);
					fclose(configFile);
				}
			}
			else if (!choice)
			{
				configFile = fopen(string_nKaruga_config, "w+");
				if (configFile)
				{
					writeToConfig(configFile);
					fseek(configFile, 0, SEEK_SET);
					readFromConfig(configFile);
					fclose(configFile);
				}
				Level::soundSystem->quickPlaySFX(sound_entries[SD_MENU_START]);
				clearBufferB();
				updateScreen();
				SDL_Delay(1000);
				G_fireback = G_difficulty > 0;
				G_hardMode = G_difficulty == 2;
				playGame();
				openedMenu = false;
			}
		}
		updateScreen();
		
		if(isKeyPressed(SDL_SCANCODE_ESCAPE))
			donePlaying = true;
	}
	
	// delete Level::soundSystem;
	delete G_particles;
	delete DC;
	
	deinitExplosionEngine();
	deinitBuffering();
	
	return 0;
}

void musicFaded()
{
	Level::phase = PHASE_BOSSCINEMATIC;
}

void bossIntroDone(int channel)
{
	if (channel == G_bossIntroChannel)
	{
		if (G_runBoss) Level::phase = PHASE_BOSSFIGHT;
		G_runBoss = false;
	}
}

void playGame()
{
	KeyEvent kEv = 0;
	int pauseTimer = 0;
	int x = 0, y = 0;
	
	Rect statsRect, levelRect;
	int chainColor[3] = { 0 };
	
	// Game phase
	int bossBonus = 0;
	
	// Variables for transition animation
	int currentW = 0, currentH = 0, chapterNum = 0, dX = 0, dY = 0;
	bool drawPowerSlot = true;
	
	Level::init(1);
	
	ChainNotif chainNotifsArray[MAX_ENEMY];
	int currentNotif;
	
	Level::p->setx(itofix(160));
	Level::p->sety(itofix(180));
	
	// Reset the level stream
	Level::counter = 0;
	pauseTimer = 0;
	G_hasFiredOnce = false;
	
	G_runBoss = false;
	
	for(int i = 0; i < MAX_ENEMY; i++)
	{
		Level::enemiesArray->data[i].deactivate();
		G_killedThisFrame[i] = -1;
	}
	
	G_score = 0;
	G_power = MAX_POWER;
	for(int i = 0; i < 3; i++)
		chainColor[i] = 0;
	G_chainStatus = 0;
	G_frameChainOffset = 0;
	
	G_inChainCount = 0;
	currentNotif = 0;
	G_gpTimer = 0;
	G_minX = 0;
	G_maxX = 320;

	while(!KQUIT(kEv) && !Level::gameEnded)
	{
		G_gpTimer++;
		Level::waveTimer++;
		
		Level::enemiesArray->handle();
		
		kEv = getk();
		
		// Prevent movement and firing during transitions
		if(Level::phase == PHASE_TRANSITION)
			kEv = 0;
		
		Level::p->handle(kEv);
		
		Level::bArray->handle();
		
		// Draw everything that has to be drawn
		DC->flush();
		// Update sound system
		Level::soundSystem->update();
		
		if(Level::phase == PHASE_GAME && !Level::fightingBoss)
			Level::advanceLevel();
		else if(Level::phase == PHASE_TRANSITION)
			Level::executeIntro();
		else if(Level::phase == PHASE_BOSSFIGHT)
		{
			if (!Level::soundSystem->musicPlaying())
				Level::soundSystem->playBgMusic(NULL, music_entries[BGM_CHAPTER1_BOSS]);
			if(Level::be->handle())
			{
				Level::phase = PHASE_BOSSEXPLODEINIT;
				Level::fightingBoss = false;
				Level::enemiesArray->destroyAllEnemies();
				Level::bArray->clear();
			}
		}
		
		if (Level::fightingBoss)
		{
			if (Level::phase == PHASE_BOSSCINEMATIC && G_bossIntroChannel == -2)
			{
				G_bossIntroChannel = Level::soundSystem->quickPlaySFX(sound_entries[SD_BOSS_ALERT]);
				Mix_ChannelFinished(bossIntroDone);
				G_runBoss = true;
				if (G_bossIntroChannel == -1)
				{
					printf("Error happened : %s\n", Mix_GetError());
				}
			}
		}
		
		// Put that here so that the game is fully drawn when the game over screen shows
		// Wait for the ship's explosion and the small delay to complete
		if(!Level::p->getLives() && Level::p->deathCounter > 12)
		{
			// #####################
			// TODO : save highscore
			// #####################
			
			bool hasPressed = false;
			// Display "continue" screen
			int x = (320 - stringWidth(string_continue)) / 2;
			int y = 120;
			drawString(&x, &y, x, string_continue, 0xffff, 0x0000);
			updateScreen();
			while(!hasPressed)
			{
				updateKeys();
				if(isKeyPressed(SDL_SCANCODE_RETURN))
				{
					hasPressed = true;
					// - initialise a new ship
					Level::p->reset();
					// - destroy all bullets and dead enemies data (no posthumous scoring)
					Level::bArray->clear();
					Level::enemiesArray->deadEnemies.clear();
					// - score and power set to 0
					G_score = 0; // ouch
					G_power = 0;
					// - chains to 0
					G_chainStatus = 0;
					G_frameChainOffset = 0;
					G_inChainCount = 0;
					G_maxChain = 0; // I know that this one hurts but it has to be done ;_;
					// - DO NOT RESET DOT EATER ACHIEVEMENT
				}
				else if(isKeyPressed(SDL_SCANCODE_ESCAPE))
				{
					hasPressed = true;
					Level::gameEnded = 1;
					wait_no_key_pressed(SDL_SCANCODE_ESCAPE);
				}
			}
		}
		
		// Things drawn passed this point MUST NOT be candidates
		
		G_particles->handle();
		
		// Draw score and chains
		statsRect.x = statsRect.y = 0;
		if (!Level::fightingBoss)
		{
			drawStringF(&statsRect.x, &statsRect.y, 0, 0xffff, 0, "Score : %d\n\n\n\nCH %d", G_score, G_chainStatus);
			// Draw chain count
			for (int i = 0, j = 0; i < G_inChainCount; i++, j += 18)
				drawSprite(image_entries[chainColor[i] == LIGHT ? image_LUT_chain_hit_light : image_LUT_chain_hit_shadow], j, 12, 0, 0);
		}
		else
		{
			drawStringF(&statsRect.x, &statsRect.y, 0, 0xffff, 0, "Score : %d\nTime : %d", G_score, Level::be->getTimeout());
		}

		// Draw explosions
		Level::enemiesArray->handleExplosions();

		// Draw power
		for (int i = MAX_FRAGMENT - 1; i >= 0; i--)
		{
			drawSprite(image_entries[image_LUT_powerslot], 5, i * 14 + 40, 0, 0);
			for (int j = 0; j < 10; j++)
			{
				if (G_power > (MAX_FRAGMENT - 1 - i) * 10 + j)
					drawHLine(i * 14 + 40 + 10 - j, 5 + power_fill_offsets[j * 2], 5 + power_fill_offsets[j * 2 + 1],
					drawPowerSlot || G_power < (MAX_FRAGMENT - i) * 10 ? (Level::p->getPolarity() ? 0xf800 : 0x3ff) : 0xffff);
				else
					break;
			}
		}
		drawPowerSlot = (G_gpTimer / 4) % 2;

		// Draw score-chaining notifs
		if (!Level::fightingBoss)
			for (int i = 0; i < MAX_ENEMY; i++)
				chainNotifsArray[i].handle();

		// Draw remaining lives
		drawSprite(image_entries[image_LUT_lives], 0, 224, 0, 0);
		statsRect.x = image_entries[image_LUT_lives][0] + 2;
		statsRect.y = 226;
		drawChar(&statsRect.x, &statsRect.y, 0, 'x', 0xffff, 0);
		drawDecimal(&statsRect.x, &statsRect.y, max(0, Level::p->getLives() - 1), 0xffff, 0);

		// Overwrite all of that
		if (Level::phase == PHASE_RESULTS)
		{
			if (currentH == 120 && currentW == 160)
			{
				clearBufferB();
				statsRect.x = (320 - stringWidth(string_results[0])) / 2;
				statsRect.y = 16;
				drawString(&statsRect.x, &statsRect.y, (320 - stringWidth(string_results[1])) / 2, string_results[0], 0xffff, 0);
				if (G_gpTimer > 64)
				{
					Level::soundSystem->fadeOutMusic(2000, NULL);
					// Boss bonus
					drawString(&statsRect.x, &statsRect.y, (320 - numberWidth(Level::be->getTimeout() * 10000)) / 2, string_results[1], 0xffff, 0);
					drawDecimal(&statsRect.x, &statsRect.y, Level::be->getTimeout() * 10000, 0xffff, 0);
				}
				if (G_gpTimer > 128)
				{
					G_score += bossBonus;
					bossBonus = 0;
					// Score
					statsRect.x = (320 - stringWidth(string_results[2])) / 2;
					statsRect.y += 16;
					drawString(&statsRect.x, &statsRect.y, (320 - numberWidth(G_score)) / 2, string_results[2], 0xffff, 0);
					drawDecimal(&statsRect.x, &statsRect.y, G_score, 0xffff, 0);
					statsRect.x = (320 - stringWidth(string_results[3]) - stringWidth(string_results[4]) - numberWidth(G_maxChain)) / 2;
					statsRect.y += 16;
					drawString(&statsRect.x, &statsRect.y, 0, string_results[3], 0xffff, 0);
					drawDecimal(&statsRect.x, &statsRect.y, G_maxChain, 0xffff, 0);
					drawString(&statsRect.x, &statsRect.y, (320 - stringWidth(string_results[5])) / 2, string_results[4], 0xffff, 0);
					// Grade
					drawString(&statsRect.x, &statsRect.y, (320 - stringWidth("Dot eater !")) / 2, string_results[5], 0xffff, 0);
					if (!G_hasFiredOnce)
						drawString(&statsRect.x, &statsRect.y, statsRect.x, "Dot eater !", 0xffff, 0);
				}
				if (G_gpTimer > 192 && KFIRE(kEv))
				{
					G_maxChain = 0;
					Level::phase = PHASE_GAME;
				}
			}
			else
			{
				if (currentH < 120) currentH++;
				else if (currentW < 160) currentW += 2;

				fillRect(160 - currentW, 120 - currentH, currentW * 2 + 1, currentH * 2, 0);
				G_gpTimer = 0;
			}
		}
		else if (Level::phase == PHASE_BOSSCINEMATIC)
		{
			drawSprite(image_entries[image_LUT_bossWarning], 0, 72, 0, 0);
			// Level::soundSystem->bgChannel->setVolume(1.);
		}
		else if (Level::phase == PHASE_BOSSEXPLODEINIT)
		{
			initExplosionEffect(160, 120, 500, 0);
			Level::phase = PHASE_BOSSEXPLODE;
			bossBonus = Level::be->getTimeout() * 10000;
		}
		else if (Level::phase == PHASE_BOSSEXPLODE)
		{
			if (updateExplosionEffect())
				Level::phase = PHASE_GAME;
		}

		if (!pauseTimer)
		{
			if (KPAUSE(kEv))
			{
				Level::soundSystem->setPausedBgMusic(true);
				// Pause the game until another pauseKey is pressed
				wait_no_key_pressed(G_pauseKey);

				// Display a "paused" box. It will be cleared in the next frame.
				int x = (320 - stringWidth("Paused")) / 2, y = 116;
				fillRect(60, 100, 200, 40, 0xffff);
				drawHLine(100, 60, 260, 0);
				drawHLine(140, 60, 260, 0);
				drawVLine(60, 100, 140, 0);
				drawVLine(260, 100, 140, 0);
				drawString(&x, &y, 0, "Paused", 0, 0xffff);
				updateScreen();

				while (!isKeyPressed(G_pauseKey))
				{
					updateKeys();
					constrainFrameRate(10);
					if (isKeyPressed(SDL_SCANCODE_ESCAPE))
					{
						kEv = 128; // KQUIT
						break;
					}
				}
				wait_no_key_pressed(G_pauseKey);
				pauseTimer = 5;
				Level::soundSystem->setPausedBgMusic(false);
			}
		}
		else
			pauseTimer--;

		renderExplosionEffect();

		updateScreen();

		// The background is dispayed after to keep the enemies(power slots, player Level::p ...) 
		// on the screen when pausing the game
		// Display a scrolling background
		Level::updateBg();
		
		constrainFrameRate(FPS);
		
		// handle chaining
		if(!Level::fightingBoss)
		{
			for(int i = 0; i < MAX_ENEMY; i++)
			{
				if(G_killedThisFrame[i] != -1)
				{
					if(G_inChainCount == 3) G_inChainCount = 0;
					
					if(G_inChainCount)
					{
						if(chainColor[G_inChainCount - 1] != G_killedThisFrame[i])
						{
							G_inChainCount = 0;
							G_chainStatus = 0;
						}
					}
					
					chainColor[G_inChainCount] = G_killedThisFrame[i];
					G_inChainCount++;
					
					if(G_inChainCount == 3)
					{
						G_score += 100 * (1 << min(G_chainStatus, 8));
						for(int j = 0; j < MAX_ENEMY; j++)
						{
							if(Level::enemiesArray->deadEnemies.relevant[j])
							{
								if(j == i)
								{
									chainNotifsArray[currentNotif].activate(Level::enemiesArray->deadEnemies.x[j], Level::enemiesArray->deadEnemies.y[j], 100 * (1 << min(G_chainStatus, 8)));
									currentNotif = (currentNotif + 1) % MAX_ENEMY;
								}
								Level::enemiesArray->deadEnemies.relevant[j] = false;
							}
						}
						G_chainStatus++;
						Level::soundSystem->quickPlaySFX(sound_entries[SD_PLAYER_CHAIN]);
					}
					G_killedThisFrame[i] = -1;
				}
			}
			G_maxChain = max(G_chainStatus, G_maxChain);
			G_frameChainOffset = 0;
		}
	}
	
	Level::deinit();
}
