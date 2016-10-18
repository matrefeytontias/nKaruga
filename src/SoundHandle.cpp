#include "common.h"

bool SoundHandle::launchLoop;

SoundHandle::SoundHandle()
{
	Mix_Init(MIX_INIT_OGG);
	if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 1024) == -1)
	{
		printf("Can't initialize audio : %s\n", Mix_GetError());
		exit(1);
	}
	Mix_AllocateChannels(32);
	loop = NULL;
	launchLoop = false;
}

SoundHandle::~SoundHandle()
{
	Mix_Quit();
}

void SoundHandle::update()
{
	if (launchLoop)
	{
		launchLoop = false;
		Mix_HookMusicFinished(NULL);
		Mix_PlayMusic(loop, -1);
	}
}

int SoundHandle::quickPlaySFX(Mix_Chunk *sfx)
{
	return Mix_PlayChannel(-1, sfx, 0);
}

// Hook to branch the loop part of a music after the main part is done playing
void SoundHandle::hook_branch()
{
	launchLoop = true;
}

// loopM is guaranteed to be non-NULL
// If the songs consists only in a loop, mainM == NULL
int SoundHandle::playBgMusic(Mix_Music *mainM, Mix_Music *loopM)
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

void SoundHandle::setPausedBgMusic(bool pause)
{
	if (pause)
		Mix_PauseMusic();
	else
		Mix_ResumeMusic();
}

int SoundHandle::fadeOutMusic(int ms, void(*callback)())
{
	Mix_HookMusicFinished(callback);
	return Mix_FadeOutMusic(ms);
}

int SoundHandle::stopBgMusic()
{
	Mix_HookMusicFinished(NULL);
	return Mix_HaltMusic();
}

bool SoundHandle::musicPlaying()
{
	return Mix_PlayingMusic();
}