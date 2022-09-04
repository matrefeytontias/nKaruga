#include "GameSystems.hpp"

#include <cassert>

#include "graphics/Particles.hpp"
#include "handlers/DrawingCandidates.hpp"
#include "handlers/SoundHandler.hpp"

std::unique_ptr<GameSystems> GS;
std::unique_ptr<GameParameters> GP;

void GameSystems::init()
{
	assert(GS == nullptr);
	GS = std::unique_ptr<GameSystems>(new GameSystems);
}

GameSystems::GameSystems() : minX(0), maxX(Constants::SCREEN_WIDTH), chapterTimer(0), skipFrame(0), killedThisFrame(), chainStatus(0),
inChainCount(0), maxChain(0), score(0), power(0), hasFiredOnce(false), bossIntroChannel(0)
{
	DC = std::make_unique<DrawingCandidates>();
	particles = std::make_unique<Particles>();
	soundSystem = std::make_unique<SoundHandler>();
}

void GameSystems::setPlayAreaSize(int _x1, int _x2)
{
	minX = _x1;
	maxX = _x2;
}

int GameSystems::getChapterTimer() const
{
	return chapterTimer;
}

void GameParameters::init()
{
	assert(GP == nullptr);
	GP = std::unique_ptr<GameParameters>(new GameParameters);
}

GameParameters::GameParameters() : usingArrows(true), difficulty(Constants::DifficultySetting::EASY),
fireback(false), hardMode(false), keys({})
{ }
