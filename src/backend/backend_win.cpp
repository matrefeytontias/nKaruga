#include "backend/backends.h"

#include <SDL2/SDL.h>

//
// Windows backend, backed by SDL2.
//

const Uint8* keysArray = nullptr;
int numKeys = 0;

void Backend::init()
{
	keysArray = SDL_GetKeyboardState(&numKeys);
}

void Backend::updateKeys()
{
	SDL_PumpEvents();
}

bool Backend::isKeyPressed(key_t k)
{
	return !!keysArray[k];
}

void Backend::waitNoKeyPressed(key_t k)
{
	for (int i = 0; i < numKeys; i++)
	{
		if (keysArray[i])
		{
			i = 0;
			updateKeys();
		}
	}
}

bool Backend::getKeyPressed(key_t& outk)
{
	for (int i = 0; i < numKeys; i++)
	{
		if (keysArray[i])
		{
			outk = static_cast<key_t>(i);
			return true;
		}
	}
	return false;
}

void Backend::getDefaultKeyBindings(KeyBindings& keys)
{
	keys.down = SDL_SCANCODE_S;
	keys.left = SDL_SCANCODE_A;
	keys.right = SDL_SCANCODE_D;
	keys.up = SDL_SCANCODE_W;
	keys.fire = SDL_SCANCODE_I;
	keys.polarity = SDL_SCANCODE_O;
	keys.fragment = SDL_SCANCODE_P;
	keys.pause = SDL_SCANCODE_M;
}

bool Backend::canBindControls()
{
	return true;
}

bool Backend::canExitGame()
{
	return true;
}
