#include "GameSystems.hpp"

#include <cassert>
#include <cstdio>

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
fireback(false), hardMode(false), keys()
{
	Backend::getDefaultActionKeyBindings(keys);
	Backend::getDefaultMovementKeyBindings(keys);
}

void GameParameters::loadSettings()
{
	FILE* in;
	fopen_s(&in, Constants::CONFIG_FILENAME, "rb");

	if (in)
	{
		keys.fire = fgetc(in);
		keys.polarity = fgetc(in);
		keys.fragment = fgetc(in);
		keys.pause = fgetc(in);
		difficulty = static_cast<Constants::DifficultySetting>(fgetc(in));
		usingArrows = !!fgetc(in);
		if (usingArrows)
			Backend::getArrowMovementKeyBindings(keys);
		else
			Backend::getDefaultMovementKeyBindings(keys);

		fclose(in);
	}
}

void GameParameters::saveSettings() const
{
	FILE* out;
	fopen_s(&out, Constants::CONFIG_FILENAME, "w");

	if (out)
	{
		fputc(keys.fire, out);
		fputc(keys.polarity, out);
		fputc(keys.fragment, out);
		fputc(keys.pause, out);
		fputc(static_cast<int>(difficulty), out);
		fputc(usingArrows, out);
		
		fclose(out);
	}
}
