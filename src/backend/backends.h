#pragma once

#include "types.h"

struct Backend
{
	struct KeyBindings
	{
		t_key down, left, right, up, fire, polarity, fragment, pause;
	};

	// Get the default key bindings for the backend.
	static void getDefaultKeyBindings(KeyBindings& keys);

	// Return true if the backend allows control rebinding.
	static bool canBindControls();
	// Return true if the backend allows closing the game back to an OS.
	static bool canExitGame();
};
