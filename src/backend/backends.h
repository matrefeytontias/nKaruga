#pragma once

#include "types.h"

struct Backend
{
	struct KeyBindings
	{
		key_t down, left, right, up, fire, polarity, fragment, pause;
	};

	// Call this once at the beginning.
	static void init();
	// Call this every frame.
	static void updateKeys();

	static bool isKeyPressed(key_t k);
	static void waitNoKeyPressed(key_t k);
	static bool getKeyPressed(key_t& outk);

	// Get the default key bindings for the backend.
	static void getDefaultKeyBindings(KeyBindings& keys);

	// Return true if the backend allows control rebinding.
	static bool canBindControls();
	// Return true if the backend allows closing the game back to an OS.
	static bool canExitGame();
};
