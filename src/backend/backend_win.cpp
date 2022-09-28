#include "backend/backends.h"

#include <SDL2/SDL.h>

//
// Windows backend, backed by SDL2.
//

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
