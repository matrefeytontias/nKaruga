#pragma once

#include <stack>
#include <vector>

#include "entities/BossEnemy.hpp"
#include "entities/Player.hpp"
#include "handlers/BulletArray.hpp"
#include "handlers/EnemiesArray.hpp"
#include "helpers/Constants.hpp"

class BackgroundScroller;

class Level
{
public:
	static void init(int n);
	static void reinit(int n);
	static void deinit();
	static void advanceLevel();
	static void (*executeIntro)(); // re-bindable
	static void updateBg();
	static Constants::GamePhase phase;
	static bool fightingBoss;
	static bool gameEnded;
	static int counter, timer, waveIndex, waveTimer; // level counter, pause timer (for wait commands), wave index, wave timer
	// Important global entities
	static Player* p;
	static BulletArray* bArray;
	static EnemiesArray* enemiesArray;
	static BossEnemy* be;
private:
	static int chapterNum;
	static Enemy* currentWaveEnemies[Constants::MAX_ENEMY]; // current wave's set of enemies (for 'killed' commands)
	static int skipCommand; // skip offset (for skip commands)
	static bool continueParsing;
	static std::vector<BackgroundScroller> bgStack; // needs an iterator
	static std::stack<unsigned int> commandStack; // command backtracking (for repeat commands)
	static int currentW, currentH;
	static void intro1();
	static void intro2();
	static void intro3();
	static void intro4();
	static void intro5();
};