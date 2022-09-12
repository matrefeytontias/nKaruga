#pragma once

#include "types.h"

struct Backend
{
	struct KeyBindings
	{
		t_key down, left, right, up, fire, polarity, fragment, pause;
	};

	static void getDefaultActionKeyBindings(KeyBindings& keys);
	static void getDefaultMovementKeyBindings(KeyBindings& keys);
	static void getArrowMovementKeyBindings(KeyBindings& keys);
};
