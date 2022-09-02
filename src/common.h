#pragma once

#ifndef M_PI
#define M_PI 3.1415926535897932384626433832795f
#endif

// KeyEvent
#define KDOWN(x) (x & 1)
#define KLEFT(x) (x & 2)
#define KRIGHT(x) (x & 4)
#define KUP(x) (x & 8)
#define KFIRE(x) (x & 16)
#define KPOLARITY(x) (x & 32)
#define KPOWER(x) (x & 64)
#define KQUIT(x) (x & 128)
#define KPAUSE(x) (x & 256)

/* UTILS */

// Game phases
enum
{
	PHASE_GAME,
	PHASE_TRANSITION,
	PHASE_BOSSCINEMATIC,
	PHASE_BOSSFIGHT,
	PHASE_BOSSEXPLODEINIT,
	PHASE_BOSSEXPLODE,
	PHASE_RESULTS,
};

// Enemy types
enum
{
	TYPE_ENEMY,
	TYPE_PROP,
	TYPE_BREAKABLE_PROP
};

// /!\ Fixed-point value
#define FCAMERA_SPEED (itofix(1) / 3)
// /!\ integer value
#define INV_CAMERA_SPEED 3

// Camera relations
enum
{
	CAMREL_NONE,
	CAMREL_ABSOLUTE,
	CAMREL_RELATIVE
};
