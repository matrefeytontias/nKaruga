#include "level/Level.hpp"

#include "utils.hpp"
#include "entities/BossEnemy.hpp"
#include "entities/Player.hpp"
#include "graphics/BackgroundScroller.hpp"
#include "graphics/Particles.hpp"
#include "handlers/BulletArray.hpp"
#include "handlers/EnemiesArray.hpp"
#include "handlers/DrawingCandidates.hpp"
#include "helpers/Constants.hpp"
#include "helpers/math.hpp"
#include "n2DLib/n2DLib.hpp"

#include "gfx/kanji.h"
#include "gfx/end_of_demo.h"
#include "level/levels.h"

void (*Level::executeIntro)() = NULL;
int Level::phase = 0;
bool Level::fightingBoss;
bool Level::gameEnded;
int Level::counter, Level::timer, Level::waveIndex, Level::waveTimer;
Player* Level::p;
BulletArray* Level::bArray;
EnemiesArray* Level::enemiesArray;
BossEnemy* Level::be;
SoundHandler* Level::soundSystem;
int Level::chapterNum;
Enemy* Level::currentWaveEnemies[Constants::MAX_ENEMY]; // current wave's set of enemies (for 'killed' commands)
int Level::skipCommand; // skip offset (for skip commands)
bool Level::continueParsing;
std::vector<BackgroundScroller> Level::bgStack; // needs an iterator
std::stack<unsigned int> Level::commandStack; // command backtracking (for repeat commands)
int Level::currentW, Level::currentH;

void Level::init(int n)
{
	p = new Player();
	bArray = new BulletArray();
	enemiesArray = new EnemiesArray();
	be = new BossEnemy();
	soundSystem = new SoundHandler();
	counter = 0;
	reinit(n);
}
void Level::deinit()
{
	bgStack.clear();
	while(!commandStack.empty()) commandStack.pop();
	soundSystem->stopBgMusic();
	delete p;
	delete bArray;
	delete enemiesArray;
	delete be;
	delete soundSystem;
}

void Level::reinit(int n)
{
	chapterNum = n;
	phase = PHASE_GAME;
	fightingBoss = false;
	timer = waveIndex = waveTimer = 0;
	skipCommand = 0;
	gameEnded = false;
	continueParsing = true;
	currentW = 0;
	switch(n)
	{
	case 0:
		executeIntro = intro1;
		break;
	case 1:
		executeIntro = intro2;
		break;
	case 2:
		executeIntro = intro3;
		break;
	case 3:
		executeIntro = intro4;
		break;
	case 4:
		executeIntro = intro5;
		break;
	}
}

void Level::advanceLevel()
{
	if(!timer)
	{
		do
		{
			continueParsing = false;
			// Load the current byte from the level stream
			commandStack.push(counter);
			int currentLevelByte = levelStream[counter];
			if(currentLevelByte == LVLSTR_CMD)
			{
				counter++;
				if (skipCommand)
				{
					counter += levelStream[counter] + 1;
					skipCommand--;
					continueParsing = true;
				}
				else
				{
					counter++;
					currentLevelByte = levelStream[counter];
					if (currentLevelByte == LVLSTR_NEWWAVE)
					{
						// Start a new wave
						waveTimer = 0;
						for (int i = 0; i < waveIndex; i++)
							currentWaveEnemies[i] = NULL;
						waveIndex = 0;
						counter++;
						enemiesArray->resetEnemyCounter();
						// Reset relative camera
						DC->cam.relX = DC->cam.relY = 0;
					}
					else if (currentLevelByte == LVLSTR_NEWCAMERA)
					{
						// Follow a new camera path
						DC->loadCameraPath(levelStream[++counter]);
						counter++;
					}
					else if (currentLevelByte == LVLSTR_WAIT)
					{
						// Wait some frames
						timer = levelStream[counter + 1];
						counter += 2;
					}
					else if (currentLevelByte == LVLSTR_WAITABS)
					{
						// Wait until G_gpTimer is above a certain value
						if (G_gpTimer < levelStream[counter + 1])
							counter -= 2;
						else
							counter += 2;
					}
					else if (currentLevelByte == LVLSTR_WAITCAMERA)
					{
						// Wait until the absolute camera reaches a given point
						if (DC->cam.absX == levelStream[counter + 1] && DC->cam.absY == levelStream[counter + 2])
							counter += 3;
						else
							counter -= 2;
					}
					else if (currentLevelByte == LVLSTR_SKIP)
					{
						if (waveTimer > levelStream[++counter])
							skipCommand = levelStream[counter + 1];
						counter += 2;
						continueParsing = true;
					}
					else if (currentLevelByte == LVLSTR_SKIPABS)
					{
						if (G_gpTimer > levelStream[++counter])
							skipCommand = levelStream[counter + 1];
						counter += 2;
						continueParsing = true;
					}
					else if (currentLevelByte == LVLSTR_REPEATABS)
					{
						printf("Global timer : %d\n", G_gpTimer / Constants::FPS);
						if (G_gpTimer < levelStream[counter + 1])
						{
							int amount = levelStream[counter + 2];
							for(int i = 0; i < amount; i++)
								commandStack.pop();
							counter = commandStack.top();
						}
						else
							counter += 3;
					}
					else if (currentLevelByte == LVLSTR_KILLED)
					{
						// Wait for every enemy to be killed before progressing
						if (enemiesArray->enemiesKilled()) counter++;
						else counter -= 2;
					}
					else if (currentLevelByte == LVLSTR_BACKGROUND)
					{
						BackgroundScroller bg(LUTs::backgroundImage(static_cast<LUTs::BgImageId>(levelStream[counter + 1])),
											  levelStream[counter + 2], levelStream[counter + 3], levelStream[counter + 4],
											  levelStream[counter + 5], levelStream[counter + 6]);
						bgStack.push_back(bg);
						counter += 7;
						continueParsing = true;
					}
					else if (currentLevelByte == LVLSTR_RESETBG)
					{
						bgStack.clear();
						counter++;
					}
					else if (currentLevelByte == LVLSTR_MUSIC)
					{
						soundSystem->playBgMusic(LUTs::music(static_cast<LUTs::MusicId>(levelStream[counter + 1])),
												 LUTs::music(static_cast<LUTs::MusicId>(levelStream[counter + 2])));
						counter += 3;
					}
					else if (currentLevelByte == LVLSTR_REINIT)
					{
						chapterNum = levelStream[++counter];
						// Prepare for a new chapter
						enemiesArray->destroyAllEnemies();
						bArray->clear();
						// Reset everything chains-related
						G_chainStatus = 0;
						G_frameChainOffset = 0;
						G_inChainCount = 0;
						G_power = 0;
						clearBufferB();
						updateScreen();
						SDL_Delay(1000);
						counter++;
						// Reset all camera position
						DC->cam.absX = DC->cam.absY = DC->cam.relX = DC->cam.relY = 0;
						// And here we go for a new chapter
						G_gpTimer = 0;
						reinit(chapterNum);
						
					}
					else if (currentLevelByte == LVLSTR_CHAPTER)
					{
						// Start a new chapter
						// Set up transition
						phase = PHASE_TRANSITION;
						currentW = 0;
						G_gpTimer = 0;
						counter++;
					}
					else if (currentLevelByte == LVLSTR_JOINT)
					{
						// Constrain an enemy to another
						currentWaveEnemies[levelStream[counter + 1]]->joint(currentWaveEnemies[levelStream[counter + 2]],
							levelStream[counter + 3],levelStream[counter + 4], 0, 0, -1, -1,
							LUTs::baseImage(LUTs::BaseImageId::NONE), LUTs::baseImage(LUTs::BaseImageId::NONE),
							levelStream[counter + 5]);
						counter += 6;
					}
					else if (currentLevelByte == LVLSTR_BOSS)
					{
						// # TODO : BETTER WAY TO DO THIS #
						// Cinematic when the music is faded
						fightingBoss = true;
						soundSystem->stopBgMusic();
						phase = PHASE_BOSSCINEMATIC;
						G_bossIntroChannel = -2; // reinit the boss intro channel
						G_gpTimer = 0;
						// fight boss
						counter++;
						BossData bossData = createBossData(levelStream[counter]);
						be->activate(&bossData);
						counter++;
					}
					else if (currentLevelByte == LVLSTR_BKPT)
					{
						// Debug stuff
						printf("Current wave timer : %d\nGlobal timer : %d\nCurrent wave index : %d\n",
							waveTimer, G_gpTimer, waveIndex);
						for (int i = 0; i < Constants::MAX_ENEMY; i++)
						{
							Enemy *e = &(enemiesArray->data[i]);
							if (e->isActive())
								printf("Enemy %d :\n\tProp : %d\n\tX,Y : %d, %d\n", i, e->isProp(),  fixtoi(e->getx()), fixtoi(e->gety()));
						}
						counter++;
					}
					else
					{
						// You messed up your level stream bro
						printf("Error : %d : unknown command !\n", currentLevelByte);
					}
				}
			}
			else if(currentLevelByte == LVLSTR_CHAPTEREND)
			{
				phase = PHASE_RESULTS;
				currentW = 0;
				currentH = 0;
				G_gpTimer = 0;
				counter++;
			}
			else if(currentLevelByte == LVLSTR_END)
			{
				// End of the game
				// #########
				// TEMPORARY
				// #########
				// Display the "end of demo" screen for 5 seconds and return to main menu
				clearBufferB();
				drawSprite(image_end_of_demo, 0, 0, 0, 0);
				updateScreen();
				SDL_Delay(5000);
				gameEnded = true;
			}
			else
			{
				// Dunno what it is ? Then it's an enemy by default
				if (!skipCommand)
				{
					bool prop = levelStream[counter + 8] != TYPE_ENEMY;
					Enemy *e = enemiesArray->add(itofix(levelStream[counter]), itofix(levelStream[counter + 1]), levelStream[counter + 2],
						levelStream[counter + 3], levelStream[counter + 4], waveIndex, levelStream[counter + 5],
						levelStream[counter + 6], levelStream[counter + 7], false, levelStream[counter + 8]);
					if (!prop)
					{
						currentWaveEnemies[waveIndex] = e;
						waveIndex++;
					}
				}
				else
					skipCommand--;
				counter += 9;
				continueParsing = true;
			}
		} while(continueParsing);
	}
	else
		timer--;
}

void Level::updateBg()
{
	for(auto& bg : bgStack)
	{
		bg.draw();
		bg.update();
	}
}

// Introductions
static const char *levelStrs[5] = { "Chapter 1\nIdeal", "Chapter 2\nTrial", "Chapter 3\nFaith", "Chapter 4\nReality", "Chapter 5\nMetempsychosis" };
static unsigned short *levelKanjis[2] = { image_kanji_1, image_kanji_2 };

void Level::intro1()
{
	static int dX = 0, dY = 0;
	fillRect(0, 0, currentW, 240, 0);
	if(!currentW)
	{
		#define TRANSLATE 120
		dX = (itofix(220) - p->getx()) / TRANSLATE;
		dY = (itofix(180) - p->gety()) / TRANSLATE;
		currentW++;
	}
	else if(currentW < 120)
	{
		currentW++;
	}
	else
	{
		if (G_gpTimer == currentW + 1) G_particles->pulse(p->getx(), p->gety(), p->getPolarity());
		int x = 30;
		int y = 60;
		drawString(&x, &y, x, levelStrs[chapterNum], 0xffff, 0);
		drawSprite(levelKanjis[chapterNum], 30, 80, 0, 0);
	}
	if(G_gpTimer > 384)
	{
		// Reset all camera position
		DC->cam.absX = DC->cam.absY = DC->cam.relX = DC->cam.relY = 0;
		phase = PHASE_GAME;
	}
	else if(G_gpTimer < TRANSLATE)
	{
		p->setx(p->getx() + dX);
		p->sety(p->gety() + dY);
	}
}

void Level::intro2()
{
	static Enemy *ld1 = NULL, *ld2 = NULL, *rd1 = NULL, *rd2 = NULL;
	static bool dash = false;
	
	if(!currentW)
	{
		ld1 = enemiesArray->add(itofix(-166 + 83), itofix(120), 1, static_cast<int>(LUTs::BaseImageId::DOOR_LEFT), Pattern_2_leftDoor, 0, Constants::LIGHT, false, 0, true, TYPE_ENEMY);
		ld2 = enemiesArray->add(itofix(-166 + 83), itofix(-120), 1, static_cast<int>(LUTs::BaseImageId::DOOR_LEFT), Pattern_2_leftDoor, 0, Constants::LIGHT, false, 0, true, TYPE_ENEMY);
		rd1 = enemiesArray->add(itofix(320 + 83), itofix(120), 1, static_cast<int>(LUTs::BaseImageId::DOOR_RIGHT), Pattern_2_rightDoor, 0, Constants::LIGHT, false, 0, true, TYPE_ENEMY);
		rd2 = enemiesArray->add(itofix(320 + 83), itofix(-120), 1, static_cast<int>(LUTs::BaseImageId::DOOR_RIGHT), Pattern_2_rightDoor, 0, Constants::LIGHT, false, 0, true, TYPE_ENEMY);
	}
	else if(currentW >= 120 && G_gpTimer == currentW + 1)
	{
		ld1->internal[1] = itofix(8);
		ld2->internal[1] = itofix(8);
		rd1->internal[1] = itofix(8);
		rd2->internal[1] = itofix(8);
	}
	dash = ld1->internal[1] > itofix(1);
	intro1();
	
	if(dash)
	{
		for(int i = 0; i < 4; i++)
		{
			G_particles->add(p->getx(), p->gety() + itofix(14), (rand() % 32) + 48, (rand() % itofix(2)) + itofix(2), p->getPolarity(), Constants::FPS);
		}
	}
}

void Level::intro3()
{
}

void Level::intro4()
{
}

void Level::intro5()
{
}
