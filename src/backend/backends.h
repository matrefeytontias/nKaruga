#pragma once

#include "types.h"

struct Backend
{
	struct KeyBindings
	{
		t_key down, left, right, up, fire, polarity, fragment, pause;
	};

	// Get the default keys associated to the fire, polarity switch, fragment
	// firing and pausing actions.
	static void getDefaultActionKeyBindings(KeyBindings& keys);
	// Get the default non-arrow keys associated to movement (down, left, right, up).
	static void getDefaultMovementKeyBindings(KeyBindings& keys);
	// Get the arrow keys associated to movement (down, left, right, up).
	static void getArrowMovementKeyBindings(KeyBindings& keys);

	// Return true if the backend allows control rebinding.
	static bool canBindControls();
};
