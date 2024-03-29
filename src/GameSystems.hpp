#pragma once

#include <memory>

#include "helpers/Constants.hpp"

class DrawingCandidates;
class Particles;
class SoundHandler;

// Used for keeping track of various elements pertaining
// to a running game.
class GameSystems
{
public:
	static void init();

	std::unique_ptr<DrawingCandidates> DC;
	std::unique_ptr<Particles> particles;
	std::unique_ptr<SoundHandler> soundSystem;

	void setPlayAreaSize(int minX, int maxX);
	int getChapterTimer() const;

private:
	GameSystems();

// Temporary public
// TODO : refactor and take out unneeded variables
public:
	// Screen boundaries, changes on special occasions.
	int minX, maxX;
	// General-purpose timer, reset to 0 at the beginning of each chapter.
	int chapterTimer;
	// Skip the current frame for rendering if this is non-zero.
	int skipFrame;
	// All the enemies that died in the same frame.
	// TODO : use MAX_KILLED_ENEMIES_PER_FRAME
	int killedThisFrame[Constants::MAX_ENEMY];
	// TODO : pretty sure this is not used anywhere
	int frameChainOffset;
	// How many chains were performed so far.
	int chainStatus;
	// How many enemies killed in this current chain.
	int inChainCount;
	// Max chains ever achieved in the level.
	int maxChain;
	int score;
	// Power bar filling.
	int power;
	// Keep track for dot eater achievement.
	bool hasFiredOnce;
	// Sound channel for the boss intro sound.
	int bossIntroChannel;
	// Tell if the boss has to be ran soon (see main.cpp)
	bool runBoss;
};

// Not set until GameSystems::init() is called.
extern std::unique_ptr<GameSystems> GS;

// Holds various parameters that act on the game's mechanics.
class GameParameters
{
public:
	static void init();

	// Whether the control scheme uses arrow keys or custom-bound keys.
	bool usingArrows;
	// The current game's difficulty setting.
	Constants::DifficultySetting difficulty;
	// Whether enemies fire suicide bullets (normal mode and +).
	// TODO : don't need that
	bool fireback;
	bool hardMode;
	// Whether or not to display the scrolling background.
	// TODO : legacy option for monochrome TI-Nspire, decide
	// what to do with it.
	bool displayBg;

	// TODO : handle keybinding and default bindings via backend
	struct KeyBindings
	{
		t_key down, left, right, up, fire, polarity, fragment, pause;
	};
	
	KeyBindings keys;
private:
	GameParameters();
};

// Not set until `GameParameters::init()` is called.
extern std::unique_ptr<GameParameters> GP;
