#pragma once

#include <SDL2/SDL_mixer.h>

// Sound handler
class SoundHandler
{
public:
	SoundHandler();
	~SoundHandler();
	void update();
	int quickPlaySFX(Mix_Chunk* sfx);
	int playBgMusic(Mix_Music* mainM, Mix_Music* loopM);
	void setPausedBgMusic(bool paused);
	int fadeOutMusic(int ms, void(*callback)());
	int stopBgMusic();
	bool musicPlaying();
private:
	static void hook_branch();
	Mix_Music* loop;
	static bool launchLoop;
};

