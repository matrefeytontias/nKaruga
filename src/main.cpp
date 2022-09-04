#include <SDL2/SDL.h>

#include "n2DLib/n2DLib.h"

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
#include "n2DLib/n2DLib_math.h"

#include "gfx/kanji.h"

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
	writeKeyToConfig(out, &GP->keys.fire);
	writeKeyToConfig(out, &GP->keys.polarity);
	writeKeyToConfig(out, &GP->keys.fragment);
	writeKeyToConfig(out, &GP->keys.pause);
	fputc(static_cast<int>(GP->difficulty), out);
	fputc(GP->usingArrows, out);
	fputc(GP->displayBg, out);
}

inline void readFromConfig(FILE* in)
{
	readKeyFromConfig(in, &GP->keys.fire);
	readKeyFromConfig(in, &GP->keys.polarity);
	readKeyFromConfig(in, &GP->keys.fragment);
	readKeyFromConfig(in, &GP->keys.pause);
	GP->difficulty = static_cast<Constants::DifficultySetting>(fgetc(in));
	GP->usingArrows = !!fgetc(in);
	if (GP->usingArrows)
	{
		GP->keys.down = SDL_SCANCODE_DOWN;
		GP->keys.left = SDL_SCANCODE_LEFT;
		GP->keys.right = SDL_SCANCODE_RIGHT;
		GP->keys.up = SDL_SCANCODE_UP;
	}
	else
	{
		GP->keys.down = SDL_SCANCODE_S;
		GP->keys.left = SDL_SCANCODE_A;
		GP->keys.right = SDL_SCANCODE_D;
		GP->keys.up = SDL_SCANCODE_W;
	}
	GP->displayBg = !!fgetc(in);
}

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

	initBuffering();

	printf("Building game LUTs ...\n");
	LUTs::buildGameLUTs();
	printf("Done\n");

	initExplosionEngine();

	int blink = 0;
	bool donePlaying = false, openedMenu = false;
	FILE* configFile;
	// Custom keys vars
	t_key* customKeys = &GP->keys.fire;
	int choice = 0;
	
	configFile = fopen(Constants::CONFIG_FILENAME, "rb");
	if(configFile)
	{
		readFromConfig(configFile);
		fclose(configFile);
	}
	else
	{
		GP->keys.fire = SDL_SCANCODE_I;
		GP->keys.polarity = SDL_SCANCODE_O;
		GP->keys.fragment = SDL_SCANCODE_P;
		GP->keys.pause = SDL_SCANCODE_M;
		GP->keys.down = SDL_SCANCODE_S;
		GP->keys.left = SDL_SCANCODE_A;
		GP->keys.right = SDL_SCANCODE_D;
		GP->keys.up = SDL_SCANCODE_W;
	}
	
	// Mix_Volume(-1, 0);
	// Mix_VolumeMusic(0);
	
	clearBufferW();
	timer_load(1, 0);

	// TODO : do input via n2DLib
	while(!donePlaying)
	{
		const Uint8 *keys = SDL_GetKeyboardState(NULL);

		drawSprite(LUTs::baseImage(LUTs::BaseImageId::TITLESCREEN), 0, 0, 0, 0);
		if(!openedMenu)
		{
			int x = (320 - strlen(Constants::TITLE_STRING) * 8) / 2;
			int y = 160;
			if(blink % 1024 < 512)
				drawString(&x, &y, x, Constants::TITLE_STRING, 0, 0xffff);
			blink++;
			if(isKeyPressed(SDL_SCANCODE_RETURN))
			{
				wait_no_key_pressed(SDL_SCANCODE_RETURN);
				openedMenu = true;
			}
		}
		else if(openedMenu)
		{
			void* v[4] = { NULL, &GP->difficulty, &GP->usingArrows, NULL };
			MenuItem items[Constants::TITLE_OPTIONS];
			for (int i = 0; i < Constants::TITLE_OPTIONS; i++)
			{
				items[i].name = Constants::TITLE_MENU_OPTIONS[i];
				items[i].value = v[i];
				items[i].labels = NULL;
				items[i].labelsNb = 0;
			}
			items[1].labels = Constants::DIFFICULTIES_NAMES;
			items[1].labelsNb = 3;
			items[2].labels = Constants::BOOLEAN_STRINGS;
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

				for (int i = 0; i < Constants::KEYS_TO_BIND; i++)
				{
					drawString(&x, &y, 0, Constants::KEYBINDINGS_NAMES[i], 0xffff, 0);
					updateScreen();
					while (!get_key_pressed(customKeys + i)) updateKeys();
					wait_no_key_pressed(customKeys[i]);
				}
				configFile = fopen(Constants::CONFIG_FILENAME, "wb");
				if (configFile)
				{
					writeToConfig(configFile);
					fclose(configFile);
				}
			}
			else if (!choice)
			{
				configFile = fopen(Constants::CONFIG_FILENAME, "w+");
				if (configFile)
				{
					writeToConfig(configFile);
					fseek(configFile, 0, SEEK_SET);
					readFromConfig(configFile);
					fclose(configFile);
				}
				GS->soundSystem->quickPlaySFX(LUTs::sound(LUTs::SoundId::MENU_START));
				clearBufferB();
				updateScreen();
				SDL_Delay(1000);
				GP->hardMode = GP->difficulty == Constants::DifficultySetting::HARD;
				GP->fireback = GP->difficulty == Constants::DifficultySetting::NORMAL || GP->hardMode;
				playGame();
				openedMenu = false;
			}
		}
		updateScreen();
		
		if(isKeyPressed(SDL_SCANCODE_ESCAPE))
			donePlaying = true;
	}
	
	LUTs::freeGameLUTs();

	// delete GS->soundSystem;
	
	deinitExplosionEngine();
	deinitBuffering();
	
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

	// TODO : figure out if this should be here
	Level::init(1);

	ChainNotif chainNotifsArray[Constants::MAX_ENEMY];
	int currentNotif;

	Level::p->setx(itofix(160));
	Level::p->sety(itofix(180));

	// Reset the level stream
	Level::counter = 0;
	// Level::counter = 2619; // DBG : start at level 2
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
	GS->frameChainOffset = 0;
	
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
			int x = (320 - stringWidth(Constants::CONTINUE_TEXT)) / 2;
			int y = 120;
			drawString(&x, &y, x, Constants::CONTINUE_TEXT, 0xffff, 0x0000);
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
					GS->score = 0; // ouch
					GS->power = 0;
					// - chains to 0
					GS->chainStatus = 0;
					GS->frameChainOffset = 0;
					GS->inChainCount = 0;
					GS->maxChain = 0; // I know that this one hurts but it has to be done ;_;
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
		
		GS->particles->handle();
		
		// Draw score and chains
		statsRect.x = statsRect.y = 0;
		if (!Level::fightingBoss)
		{
			drawStringF(&statsRect.x, &statsRect.y, 0, 0xffff, 0, "Score : %d\n\n\n\nCH %d", GS->score, GS->chainStatus);
			// Draw chain count
			for (int i = 0, j = 0; i < GS->inChainCount; i++, j += 18)
				drawSprite(LUTs::baseImage(chainColor[i] == Constants::LIGHT ? LUTs::BaseImageId::CHAIN_HIT_LIGHT : LUTs::BaseImageId::CHAIN_HIT_SHADOW), j, 12, 0, 0);
		}
		else
		{
			drawStringF(&statsRect.x, &statsRect.y, 0, 0xffff, 0, "Score : %d\nTime : %d", GS->score, Level::be->getTimeout());
		}

		// Draw explosions
		Level::enemiesArray->handleExplosions();

		// Draw power
		for (int i = Constants::MAX_FRAGMENT - 1; i >= 0; i--)
		{
			drawSprite(LUTs::baseImage(LUTs::BaseImageId::POWERSLOT), 5, i * 14 + 40, 0, 0);
			for (int j = 0; j < 10; j++)
			{
				if (GS->power > (Constants::MAX_FRAGMENT - 1 - i) * 10 + j)
					drawHLine(i * 14 + 40 + 10 - j, 5 + Constants::POWER_SLOT_FILL_COORDINATES[j * 2], 5 + Constants::POWER_SLOT_FILL_COORDINATES[j * 2 + 1],
							  drawPowerSlot || GS->power < (Constants::MAX_FRAGMENT - i) * 10 ? (Level::p->getPolarity() ? 0xf800 : 0x3ff) : 0xffff);
				else
					break;
			}
		}
		drawPowerSlot = (GS->chapterTimer / 4) % 2;

		// Draw score-chaining notifs
		if (!Level::fightingBoss)
			for (int i = 0; i < Constants::MAX_ENEMY; i++)
				chainNotifsArray[i].handle();

		// Draw remaining lives
		{
			uint16_t* livesImg = LUTs::baseImage(LUTs::BaseImageId::LIVES);

			drawSprite(livesImg, 0, 224, 0, 0);
			statsRect.x = livesImg[0] + 2;
			statsRect.y = 226;
			drawChar(&statsRect.x, &statsRect.y, 0, 'x', 0xffff, 0);
			drawDecimal(&statsRect.x, &statsRect.y, max(0, Level::p->getLives() - 1), 0xffff, 0);
		}

		// Overwrite all of that
		if (Level::phase == Constants::GamePhase::RESULTS)
		{
			if (currentH == 120 && currentW == 160)
			{
				clearBufferB();
				statsRect.x = (320 - stringWidth(Constants::RESULTS_TEXT[0])) / 2;
				statsRect.y = 16;
				drawString(&statsRect.x, &statsRect.y, (320 - stringWidth(Constants::RESULTS_TEXT[1])) / 2, Constants::RESULTS_TEXT[0], 0xffff, 0);
				if (GS->chapterTimer > 64)
				{
					GS->soundSystem->fadeOutMusic(2000, NULL);
					// Boss bonus
					drawString(&statsRect.x, &statsRect.y, (320 - numberWidth(Level::be->getTimeout() * 10000)) / 2, Constants::RESULTS_TEXT[1], 0xffff, 0);
					drawDecimal(&statsRect.x, &statsRect.y, Level::be->getTimeout() * 10000, 0xffff, 0);
				}
				if (GS->chapterTimer > 128)
				{
					GS->score += bossBonus;
					bossBonus = 0;
					// Score
					statsRect.x = (320 - stringWidth(Constants::RESULTS_TEXT[2])) / 2;
					statsRect.y += 16;
					drawString(&statsRect.x, &statsRect.y, (320 - numberWidth(GS->score)) / 2, Constants::RESULTS_TEXT[2], 0xffff, 0);
					drawDecimal(&statsRect.x, &statsRect.y, GS->score, 0xffff, 0);
					statsRect.x = (320 - stringWidth(Constants::RESULTS_TEXT[3]) - stringWidth(Constants::RESULTS_TEXT[4]) - numberWidth(GS->maxChain)) / 2;
					statsRect.y += 16;
					drawString(&statsRect.x, &statsRect.y, 0, Constants::RESULTS_TEXT[3], 0xffff, 0);
					drawDecimal(&statsRect.x, &statsRect.y, GS->maxChain, 0xffff, 0);
					drawString(&statsRect.x, &statsRect.y, (320 - stringWidth(Constants::RESULTS_TEXT[5])) / 2, Constants::RESULTS_TEXT[4], 0xffff, 0);
					// Grade
					drawString(&statsRect.x, &statsRect.y, (320 - stringWidth("Dot eater !")) / 2, Constants::RESULTS_TEXT[5], 0xffff, 0);
					if (!GS->hasFiredOnce)
						drawString(&statsRect.x, &statsRect.y, statsRect.x, "Dot eater !", 0xffff, 0);
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

				fillRect(160 - currentW, 120 - currentH, currentW * 2 + 1, currentH * 2, 0);
				GS->chapterTimer = 0;
			}
		}
		else if (Level::phase == Constants::GamePhase::BOSSCINEMATIC)
		{
			drawSprite(LUTs::baseImage(LUTs::BaseImageId::BOSS_WARNING), 0, 72, 0, 0);
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
				wait_no_key_pressed(GP->keys.pause);

				// Display a "paused" box. It will be cleared in the next frame.
				int x = (320 - stringWidth("Paused")) / 2, y = 116;
				fillRect(60, 100, 200, 40, 0xffff);
				drawHLine(100, 60, 260, 0);
				drawHLine(140, 60, 260, 0);
				drawVLine(60, 100, 140, 0);
				drawVLine(260, 100, 140, 0);
				drawString(&x, &y, 0, "Paused", 0, 0xffff);
				updateScreen();

				while (!isKeyPressed(GP->keys.pause))
				{
					updateKeys();
					constrainFrameRate(10);
					if (isKeyPressed(SDL_SCANCODE_ESCAPE))
					{
						kEv = 128; // KQUIT
						break;
					}
				}
				wait_no_key_pressed(GP->keys.pause);
				pauseTimer = 5;
				GS->soundSystem->setPausedBgMusic(false);
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
		
		constrainFrameRate(Constants::FPS);
		displayFrameRate();
		
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
			GS->frameChainOffset = 0;
		}
	}
	
	Level::deinit();
}
