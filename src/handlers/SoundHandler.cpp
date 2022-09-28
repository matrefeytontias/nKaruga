#include "handlers/SoundHandler.hpp"

#include <iostream>

#include <SDL2/SDL_mixer.h>

bool SoundHandler::launchLoop;

SoundHandler::SoundHandler()
{
	Mix_Init(MIX_INIT_OGG);
	if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 1024) == -1)
	{
		std::cout << "Can't initialize audio : " << Mix_GetError() << std::endl;
		exit(1);
	}
	Mix_AllocateChannels(32);
	loop = nullptr;
	launchLoop = false;
}

SoundHandler::~SoundHandler()
{
	Mix_Quit();
}

void SoundHandler::update()
{
	if (launchLoop)
	{
		launchLoop = false;
		Mix_HookMusicFinished(nullptr);
		Mix_PlayMusic(loop, -1);
	}
}

int SoundHandler::quickPlaySFX(Mix_Chunk *sfx) const
{
	return Mix_PlayChannel(-1, sfx, 0);
}

// Hook to branch the loop part of a music after the main part is done playing
void SoundHandler::hook_branch()
{
	launchLoop = true;
}

// loopM is guaranteed to be non-NULL
// If the songs consists only in a loop, mainM == NULL
int SoundHandler::playBgMusic(Mix_Music *mainM, Mix_Music *loopM)
{
	int r = 0;
	if (mainM)
	{
		Mix_HookMusicFinished(hook_branch);
		loop = loopM;
		r = Mix_PlayMusic(mainM, 1);
	}
	else
		r = Mix_PlayMusic(loopM, -1);
	return r;
}

void SoundHandler::setPausedBgMusic(bool pause) const
{
	if (pause)
		Mix_PauseMusic();
	else
		Mix_ResumeMusic();
}

int SoundHandler::fadeOutMusic(int ms, void(*callback)()) const
{
	Mix_HookMusicFinished(callback);
	return Mix_FadeOutMusic(ms);
}

int SoundHandler::stopBgMusic() const
{
	Mix_HookMusicFinished(NULL);
	return Mix_HaltMusic();
}

bool SoundHandler::musicPlaying() const
{
	return Mix_PlayingMusic();
}