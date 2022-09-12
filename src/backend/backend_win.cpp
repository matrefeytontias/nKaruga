#include "backend/backends.h"

#include <SDL2/SDL.h>

//
// Windows backend, backed by SDL2.
//

void Backend::getDefaultActionKeyBindings(KeyBindings& keys)
{
	keys.fire = SDL_SCANCODE_I;
	keys.polarity = SDL_SCANCODE_O;
	keys.fragment = SDL_SCANCODE_P;
	keys.pause = SDL_SCANCODE_M;
}

void Backend::getDefaultMovementKeyBindings(KeyBindings& keys)
{
	keys.down = SDL_SCANCODE_S;
	keys.left = SDL_SCANCODE_A;
	keys.right = SDL_SCANCODE_D;
	keys.up = SDL_SCANCODE_W;
}

void Backend::getArrowMovementKeyBindings(KeyBindings& keys)
{
	keys.down = SDL_SCANCODE_DOWN;
	keys.left = SDL_SCANCODE_LEFT;
	keys.right = SDL_SCANCODE_RIGHT;
	keys.up = SDL_SCANCODE_UP;
}

bool Backend::canBindControls()
{
	return true;
}
