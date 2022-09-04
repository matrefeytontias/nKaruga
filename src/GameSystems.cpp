#include "GameSystems.hpp"

#include "graphics/Particles.hpp"
#include "handlers/DrawingCandidates.hpp"

std::unique_ptr<GameSystems> GS;
std::unique_ptr<GameParameters> GP;

void GameSystems::init()
{
	if (GS == nullptr)
		GS = std::unique_ptr<GameSystems>(new GameSystems);
	else
		// TODO : assert
		(void)0;
}

GameSystems::GameSystems() : minX(0), maxX(Constants::SCREEN_WIDTH), chapterTimer(0), skipFrame(0), killedThisFrame(), chainStatus(0),
inChainCount(0), maxChain(0), score(0), power(0), hasFiredOnce(false), bossIntroChannel(0)
{
	DC = std::make_unique<DrawingCandidates>();
	particles = std::make_unique<Particles>();
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
	if (GP == nullptr)
		GP = std::unique_ptr<GameParameters>(new GameParameters);
	else
		// TODO : assert
		(void)0;
}

GameParameters::GameParameters() : usingArrows(true), difficulty(Constants::DifficultySetting::EASY),
fireback(false), hardMode(false), displayBg(true), keys({})
{ }
