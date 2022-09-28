#include <n2DLib/n2DLib.h>
#include <n2DLib/n2DLib_math.h>

#include "GameSystems.hpp"
#include "utils.hpp"
#include "graphics/ChainNotif.hpp"
#include "graphics/ExplosionEffect.hpp"
#include "graphics/Particles.hpp"
#include "handlers/DrawingCandidates.hpp"
#include "handlers/SoundHandler.hpp"
#include "helpers/Constants.hpp"
#include "level/Level.hpp"
#include "menu/Menu.hpp"

#include "gfx/kanji.h"

// #define START_LVL2

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

int main(int argc, char** argv)
{
	UNUSED(argc);
	UNUSED(argv);

	// Init things
	// TODO : make things order-independent as right now they are not,
	// or at least throw when something is not right.
	// Dependencies left :
	// - LUTs::buildGameLUTs needs SDL_mixer

	GameSystems::init();
	GameParameters::init();

	n2D_init();

	printf("Building game LUTs ...\n");
	LUTs::buildGameLUTs();
	printf("Done\n");

	initExplosionEngine();

	int blink = 0;
	bool donePlaying = false, openedMenu = false;
	// Custom keys vars accessed all in order.
	t_key* customKeys = &GP->keys.down;
	
	GP->loadSettings();

	// Mix_Volume(-1, 0);
	// Mix_VolumeMusic(0);
	
	n2D_clearBufferW();
	n2D_timerLoad(1, 0);
	
	// Construct the main menu
	bool gameStarted = false;
	bool configuringControls = false;
	DeepMenuItem startGameItem(Constants::TITLE_MENU_OPTIONS[0], gameStarted);
	IntMenuItem difficultyItem(Constants::TITLE_MENU_OPTIONS[1], *reinterpret_cast<int*>(&GP->difficulty), static_cast<int>(Constants::DifficultySetting::HARD), Constants::DIFFICULTIES_NAMES);
	DeepMenuItem bindControlsItem(Constants::TITLE_MENU_OPTIONS[2], configuringControls);
	DeepMenuItem quitItem(Constants::TITLE_MENU_OPTIONS[3], donePlaying);
	MenuItem* mainMenuItems[Constants::TITLE_OPTIONS] = { &startGameItem, &difficultyItem, nullptr, nullptr };

	int nbOptions = 2;

	if (Backend::canBindControls())
		mainMenuItems[nbOptions++] = &bindControlsItem;
	if (Backend::canExitGame())
		mainMenuItems[nbOptions++] = &quitItem;

	Menu mainMenu = Menu(nbOptions, mainMenuItems);

	while(!donePlaying)
	{
		KeyEvent k = getk();

		n2D_drawSprite(LUTs::baseImage(LUTs::BaseImageId::TITLESCREEN), 0, 0, 0, 0);
		if(!openedMenu)
		{
			int x = (320 - CONSTEXPR_STRLEN(Constants::TITLE_STRING) * 8) / 2;
			int y = 160;
			if(blink % 1024 < 512)
				n2D_drawString(&x, &y, x, Constants::TITLE_STRING, 0, 0xffff);
			blink++;
			t_key firstKey;
			if(n2D_getKeyPressed(&firstKey))
			{
				n2D_waitNoKeyPressed(firstKey);
				openedMenu = true;
			}
		}
		else
		{
			if (mainMenu.run(k))
				donePlaying = true;

			// Bind keys to controls
			if (configuringControls)
			{
				n2D_waitNoKeyPressed(GP->keys.fire);
				n2D_clearBufferB();
				int x = 0, y = 0;
				n2D_drawString(&x, &y, 0, "Press the key you want to bind to this\naction.\n\n", 0xffff, 0);

				for (int i = 0; i < Constants::KEYS_TO_BIND; i++)
				{
					n2D_drawString(&x, &y, 0, Constants::KEYBINDINGS_NAMES[i], 0xffff, 0);
					n2D_updateScreen();
					while (!n2D_getKeyPressed(customKeys + i)) n2D_updateKeys();
					n2D_waitNoKeyPressed(customKeys[i]);
				}
				GP->saveSettings();
				configuringControls = false;
			}
			else if (gameStarted)
			{
				GP->saveSettings();
				GS->soundSystem->quickPlaySFX(LUTs::sound(LUTs::SoundId::MENU_START));
				n2D_clearBufferB();
				n2D_updateScreen();
				n2D_timerLoad(0, 1000);
				while (n2D_timerRead(0));
				GP->hardMode = GP->difficulty == Constants::DifficultySetting::HARD;
				GP->fireback = GP->difficulty == Constants::DifficultySetting::NORMAL || GP->hardMode;
				playGame();
				gameStarted = false;
				openedMenu = false;
			}
		}
		n2D_updateScreen();
	}
	
	LUTs::freeGameLUTs();

	// delete GS->soundSystem;
	
	deinitExplosionEngine();
	n2D_deinit();
	
	return 0;
}

void musicFaded()
{
	Level::phase = Constants::GamePhase::BOSSCINEMATIC;
}

void bossIntroDone(int channel)
{
	if (channel == GS->bossIntroChannel)
	{
		if (GS->runBoss) Level::phase = Constants::GamePhase::BOSSFIGHT;
		GS->runBoss = false;
	}
}

void playGame()
{
	KeyEvent kEv = 0;
	int pauseTimer = 0;
	int x = 0, y = 0;

	Rect statsRect;
	int chainColor[3] = { 0 };

	// Game phase
	int bossBonus = 0;

	// Variables for transition animation
	int currentW = 0, currentH = 0, chapterNum = 0, dX = 0, dY = 0;
	bool drawPowerSlot = true;

	// TODO : refactor so Level isn't static and its lifetime makes sense
	Level::init(1);

	ChainNotif chainNotifsArray[Constants::MAX_ENEMY];
	int currentNotif;

	Level::p->setx(itofix(160));
	Level::p->sety(itofix(180));

	// Reset the level stream
	Level::counter = 0;
#ifdef START_LVL2
	Level::counter = 2619; // DBG : start at level 2
#endif
	pauseTimer = 0;
	GS->hasFiredOnce = false;

	GS->runBoss = false;

	for (int i = 0; i < Constants::MAX_ENEMY; i++)
	{
		Level::enemiesArray->data[i].deactivate();
		GS->killedThisFrame[i] = -1;
	}
	
	GS->score = 0;
	GS->power = Constants::MAX_STORED_POWER;
	for(int i = 0; i < 3; i++)
		chainColor[i] = 0;
	GS->chainStatus = 0;
	
	GS->inChainCount = 0;
	currentNotif = 0;
	GS->chapterTimer = 0;
	GS->minX = 0;
	GS->maxX = 320;

	while(!KQUIT(kEv) && !Level::gameEnded)
	{
		GS->chapterTimer++;
		Level::waveTimer++;
		
		Level::enemiesArray->handle();
		
		kEv = getk();
		
		// Prevent movement and firing during transitions
		if(Level::phase == Constants::GamePhase::TRANSITION)
			kEv = 0;
		
		Level::p->handle(kEv);
		
		Level::bArray->handle();

		// Draw everything that has to be drawn
		GS->DC->flush();
		// Update sound system
		GS->soundSystem->update();
		
		if(Level::phase == Constants::GamePhase::PLAY && !Level::fightingBoss)
			Level::advanceLevel();
		else if(Level::phase == Constants::GamePhase::TRANSITION)
			Level::executeIntro();
		else if(Level::phase == Constants::GamePhase::BOSSFIGHT)
		{
			if (!GS->soundSystem->musicPlaying())
				GS->soundSystem->playBgMusic(NULL, LUTs::music(LUTs::MusicId::CHAPTER1_BOSS));
			if(Level::be->handle())
			{
				Level::phase = Constants::GamePhase::BOSSEXPLODEINIT;
				Level::fightingBoss = false;
				Level::enemiesArray->destroyAllEnemies();
				Level::bArray->clear();
			}
		}
		
		if (Level::fightingBoss)
		{
			if (Level::phase == Constants::GamePhase::BOSSCINEMATIC && GS->bossIntroChannel == -2)
			{
				GS->bossIntroChannel = GS->soundSystem->quickPlaySFX(LUTs::sound(LUTs::SoundId::BOSS_ALERT));
				Mix_ChannelFinished(bossIntroDone);
				GS->runBoss = true;
				if (GS->bossIntroChannel == -1)
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
			int x = (320 - n2D_stringWidth(Constants::CONTINUE_TEXT)) / 2;
			int y = 120;
			n2D_drawString(&x, &y, x, Constants::CONTINUE_TEXT, 0xffff, 0x0000);
			n2D_updateScreen();
			while(!hasPressed)
			{
				n2D_updateKeys();
				if(n2D_isKeyPressed(GP->keys.fire))
				{
					hasPressed = true;
					// - initialise a new ship
					Level::p->reset();
					// - destroy all bullets and dead enemies data (no posthumous scoring)
					Level::bArray->clear();
					Level::enemiesArray->deadEnemies.clear();
					// - score and power set to 0
					GS->score = 0; // ouch
					GS->power = 0;
					// - chains to 0
					GS->chainStatus = 0;
					GS->inChainCount = 0;
					GS->maxChain = 0; // I know that this one hurts but it has to be done ;_;
					// - DO NOT RESET DOT EATER ACHIEVEMENT
				}
				else if(n2D_isKeyPressed(GP->keys.polarity))
				{
					hasPressed = true;
					Level::gameEnded = 1;
					n2D_waitNoKeyPressed(GP->keys.polarity);
				}
			}
		}

		// Things drawn passed this point MUST NOT be candidates
		
		GS->particles->handle();
		
		// Draw score and chains
		statsRect.x = statsRect.y = 0;
		if (!Level::fightingBoss)
		{
			n2D_drawStringF(&statsRect.x, &statsRect.y, 0, 0xffff, 0, "Score : %d\n\n\n\nCH %d", GS->score, GS->chainStatus);
			// Draw chain count
			for (int i = 0, j = 0; i < GS->inChainCount; i++, j += 18)
				n2D_drawSprite(LUTs::baseImage(chainColor[i] == Constants::LIGHT ? LUTs::BaseImageId::CHAIN_HIT_LIGHT : LUTs::BaseImageId::CHAIN_HIT_SHADOW), j, 12, 0, 0);
		}
		else
		{
			n2D_drawStringF(&statsRect.x, &statsRect.y, 0, 0xffff, 0, "Score : %d\nTime : %d", GS->score, Level::be->getTimeout());
		}

		// Draw explosions
		Level::enemiesArray->handleExplosions();

		// Draw power
		for (int i = Constants::MAX_FRAGMENT - 1; i >= 0; i--)
		{
			n2D_drawSprite(LUTs::baseImage(LUTs::BaseImageId::POWERSLOT), 5, i * 14 + 40, 0, 0);
			for (int j = 0; j < 10; j++)
			{
				if (GS->power > (Constants::MAX_FRAGMENT - 1 - i) * 10 + j)
					n2D_drawHLine(i * 14 + 40 + 10 - j, 5 + Constants::POWER_SLOT_FILL_COORDINATES[j * 2], 5 + Constants::POWER_SLOT_FILL_COORDINATES[j * 2 + 1],
							  drawPowerSlot || GS->power < (Constants::MAX_FRAGMENT - i) * 10 ? (Level::p->getPolarity() ? 0xf800 : 0x3ff) : 0xffff);
				else
					break;
			}
		}
		drawPowerSlot = (GS->chapterTimer / 4) % 2;

		// Draw score-chaining notifs
		if (!Level::fightingBoss)
			for (int i = 0; i < Constants::MAX_ENEMY; i++)
				if(chainNotifsArray[i].isActive())
					chainNotifsArray[i].handle();

		// Draw remaining lives
		{
			const uint16_t* livesImg = LUTs::baseImage(LUTs::BaseImageId::LIVES);

			n2D_drawSprite(livesImg, 0, 224, 0, 0);
			statsRect.x = livesImg[0] + 2;
			statsRect.y = 226;
			n2D_drawChar(&statsRect.x, &statsRect.y, 0, 'x', 0xffff, 0);
			n2D_drawDecimal(&statsRect.x, &statsRect.y, max(0, Level::p->getLives() - 1), 0xffff, 0);
		}

		// Overwrite all of that
		if (Level::phase == Constants::GamePhase::RESULTS)
		{
			if (currentH == 120 && currentW == 160)
			{
				n2D_clearBufferB();
				statsRect.x = (320 - n2D_stringWidth(Constants::RESULTS_TEXT[0])) / 2;
				statsRect.y = 16;
				n2D_drawString(&statsRect.x, &statsRect.y, (320 - n2D_stringWidth(Constants::RESULTS_TEXT[1])) / 2, Constants::RESULTS_TEXT[0], 0xffff, 0);
				if (GS->chapterTimer > 64)
				{
					GS->soundSystem->fadeOutMusic(2000, NULL);
					// Boss bonus
					n2D_drawString(&statsRect.x, &statsRect.y, (320 - n2D_numberWidth(Level::be->getTimeout() * 10000)) / 2, Constants::RESULTS_TEXT[1], 0xffff, 0);
					n2D_drawDecimal(&statsRect.x, &statsRect.y, Level::be->getTimeout() * 10000, 0xffff, 0);
				}
				if (GS->chapterTimer > 128)
				{
					GS->score += bossBonus;
					bossBonus = 0;
					// Score
					statsRect.x = (320 - n2D_stringWidth(Constants::RESULTS_TEXT[2])) / 2;
					statsRect.y += 16;
					n2D_drawString(&statsRect.x, &statsRect.y, (320 - n2D_numberWidth(GS->score)) / 2, Constants::RESULTS_TEXT[2], 0xffff, 0);
					n2D_drawDecimal(&statsRect.x, &statsRect.y, GS->score, 0xffff, 0);
					statsRect.x = (320 - n2D_stringWidth(Constants::RESULTS_TEXT[3]) - n2D_stringWidth(Constants::RESULTS_TEXT[4]) - n2D_numberWidth(GS->maxChain)) / 2;
					statsRect.y += 16;
					n2D_drawString(&statsRect.x, &statsRect.y, 0, Constants::RESULTS_TEXT[3], 0xffff, 0);
					n2D_drawDecimal(&statsRect.x, &statsRect.y, GS->maxChain, 0xffff, 0);
					n2D_drawString(&statsRect.x, &statsRect.y, (320 - n2D_stringWidth(Constants::RESULTS_TEXT[5])) / 2, Constants::RESULTS_TEXT[4], 0xffff, 0);
					// Grade
					n2D_drawString(&statsRect.x, &statsRect.y, (320 - n2D_stringWidth("Dot eater !")) / 2, Constants::RESULTS_TEXT[5], 0xffff, 0);
					if (!GS->hasFiredOnce)
						n2D_drawString(&statsRect.x, &statsRect.y, statsRect.x, "Dot eater !", 0xffff, 0);
				}
				if (GS->chapterTimer > 192 && KFIRE(kEv))
				{
					GS->maxChain = 0;
					Level::phase = Constants::GamePhase::PLAY;
				}
			}
			else
			{
				if (currentH < 120) currentH++;
				else if (currentW < 160) currentW += 2;

				n2D_fillRect(160 - currentW, 120 - currentH, currentW * 2 + 1, currentH * 2, 0);
				GS->chapterTimer = 0;
			}
		}
		else if (Level::phase == Constants::GamePhase::BOSSCINEMATIC)
		{
			n2D_drawSprite(LUTs::baseImage(LUTs::BaseImageId::BOSS_WARNING), 0, 72, 0, 0);
			// GS->soundSystem->bgChannel->setVolume(1.);
		}
		else if (Level::phase == Constants::GamePhase::BOSSEXPLODEINIT)
		{
			initExplosionEffect(160, 120, 500, 0);
			Level::phase = Constants::GamePhase::BOSSEXPLODE;
			bossBonus = Level::be->getTimeout() * 10000;
		}
		else if (Level::phase == Constants::GamePhase::BOSSEXPLODE)
		{
			if (updateExplosionEffect())
				Level::phase = Constants::GamePhase::PLAY;
		}

		if (!pauseTimer)
		{
			if (KPAUSE(kEv))
			{
				GS->soundSystem->setPausedBgMusic(true);
				// Pause the game until another pauseKey is pressed
				n2D_waitNoKeyPressed(GP->keys.pause);

				// Display a "paused" box. It will be cleared in the next frame.
				int x = (320 - n2D_stringWidth("Paused")) / 2, y = 116;
				n2D_fillRect(60, 100, 200, 40, 0xffff);
				n2D_drawHLine(100, 60, 260, 0);
				n2D_drawHLine(140, 60, 260, 0);
				n2D_drawVLine(60, 100, 140, 0);
				n2D_drawVLine(260, 100, 140, 0);
				n2D_drawString(&x, &y, 0, "Paused", 0, 0xffff);
				n2D_updateScreen();

				while (!n2D_isKeyPressed(GP->keys.pause))
				{
					n2D_updateKeys();
					n2D_constrainFrameRate(10);
					if (n2D_isKeyPressed(GP->keys.polarity))
					{
						kEv = 128; // KQUIT
						break;
					}
				}
				n2D_waitNoKeyPressed(GP->keys.pause);
				pauseTimer = 5;
				GS->soundSystem->setPausedBgMusic(false);
			}
		}
		else
			pauseTimer--;

		renderExplosionEffect();

		n2D_updateScreen();

		// The background is dispayed after to keep the enemies(power slots, player Level::p ...) 
		// on the screen when pausing the game
		// Display a scrolling background
		Level::updateBg();
		
		n2D_constrainFrameRate(Constants::FPS);
		n2D_displayFrameRate();
		
		// handle chaining
		if(!Level::fightingBoss)
		{
			for(int i = 0; i < Constants::MAX_ENEMY; i++)
			{
				if(GS->killedThisFrame[i] != -1)
				{
					if(GS->inChainCount == 3) GS->inChainCount = 0;
					
					if(GS->inChainCount)
					{
						if(chainColor[GS->inChainCount - 1] != GS->killedThisFrame[i])
						{
							GS->inChainCount = 0;
							GS->chainStatus = 0;
						}
					}
					
					chainColor[GS->inChainCount] = GS->killedThisFrame[i];
					GS->inChainCount++;
					
					if(GS->inChainCount == 3)
					{
						GS->score += 100 * (1 << min(GS->chainStatus, 8));
						for(int j = 0; j < Constants::MAX_ENEMY; j++)
						{
							if(Level::enemiesArray->deadEnemies.relevant[j])
							{
								if(j == i)
								{
									chainNotifsArray[currentNotif].activate(Level::enemiesArray->deadEnemies.x[j], Level::enemiesArray->deadEnemies.y[j], 100 * (1 << min(GS->chainStatus, 8)));
									currentNotif = (currentNotif + 1) % Constants::MAX_ENEMY;
								}
								Level::enemiesArray->deadEnemies.relevant[j] = false;
							}
						}
						GS->chainStatus++;
						GS->soundSystem->quickPlaySFX(LUTs::sound(LUTs::SoundId::PLAYER_CHAIN));
					}
					GS->killedThisFrame[i] = -1;
				}
			}
			GS->maxChain = max(GS->chainStatus, GS->maxChain);
		}
	}
	
	Level::deinit();
}
