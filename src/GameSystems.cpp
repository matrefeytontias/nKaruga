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
	Backend::init();
	GS = std::unique_ptr<GameSystems>(new GameSystems);
}

GameSystems::GameSystems() : minX(0), maxX(Constants::SCREEN_WIDTH), chapterTimer(0), skipFrame(0), killedThisFrame(), chainStatus(0),
inChainCount(0), maxChain(0), score(0), power(0), hasFiredOnce(false), bossIntroChannel(0)
{
	DC = std::make_unique<DrawingCandidates>();
	particles = std::make_unique<Particles>();
	soundSystem = std::make_unique<SoundHandler>();
}

void GameSystems::update()
{
	Backend::updateKeys();
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

GameParameters::GameParameters() : difficulty(Constants::DifficultySetting::EASY),
fireback(false), hardMode(false), keys()
{
	Backend::getDefaultKeyBindings(keys);
}

void GameParameters::loadSettings()
{
	FILE* in;
	fopen_s(&in, Constants::CONFIG_FILENAME, "rb");

	if (in)
	{
		keys.down = fgetc(in);
		keys.left = fgetc(in);
		keys.right = fgetc(in);
		keys.up = fgetc(in);
		keys.fire = fgetc(in);
		keys.polarity = fgetc(in);
		keys.fragment = fgetc(in);
		keys.pause = fgetc(in);
		difficulty = static_cast<Constants::DifficultySetting>(fgetc(in));

		fclose(in);
	}
}

void GameParameters::saveSettings() const
{
	FILE* out;
	fopen_s(&out, Constants::CONFIG_FILENAME, "wb");

	if (out)
	{
		fputc(keys.down, out);
		fputc(keys.left, out);
		fputc(keys.right, out);
		fputc(keys.up, out);
		fputc(keys.fire, out);
		fputc(keys.polarity, out);
		fputc(keys.fragment, out);
		fputc(keys.pause, out);
		fputc(static_cast<int>(difficulty), out);
		
		fclose(out);
	}
}
