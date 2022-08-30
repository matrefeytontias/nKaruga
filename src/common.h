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

// IMPORTANT
// PROPS ARE NOT CONSIDERED PARTS OF A WAVE, THUS THEIR waveIndex VARIABLE MUST NOT BE USED

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

// LUT-related

// Enemies' patterns
enum
{
	Pattern_null,
	Pattern_box,
	Pattern_prop,
	// Intro 1
	Pattern_1_1,
	Pattern_1_2,
	Pattern_1_3,
	Pattern_1_4,
	Pattern_1_5,
	Pattern_1_6,
	// Chapter 1
	Pattern_1_7,
	Pattern_1_8,
	Pattern_1_9,
	Pattern_1_10,
	Pattern_1_11,
	Pattern_1_12,
	Pattern_1_13,
	Pattern_1_14,
	Pattern_1_15,
	Pattern_1_16,
	Pattern_1_17,
	Pattern_1_18,
	Pattern_1_19,
	Pattern_1_20,
	Pattern_1_21,
	Pattern_1_boss,
	Pattern_1_bossGrenade,
	// Intro 2
	Pattern_2_1,
	Pattern_2_2,
	Pattern_2_3,
	Pattern_2_4,
	Pattern_2_5,
	Pattern_2_6,
	Pattern_2_7,
	Pattern_2_leftDoor,
	Pattern_2_rightDoor,
	// Chapter 2
	Pattern_2_8,
	Pattern_2_9,
	Pattern_2_10,
	Pattern_2_11,
	Pattern_2_12,
	Pattern_2_13,
	Pattern_2_14,
	Pattern_2_15,
	Pattern_2_16,
	Pattern_2_17,
	Pattern_2_18,
	Pattern_2_19,
	Pattern_2_20,
	Pattern_2_21,
	Pattern_2_22,
	Pattern_2_23,
	Pattern_2_24,
	Pattern_2_25,
	Pattern_2_26,
	Pattern_2_27,
	Pattern_2_28,
	Pattern_2_29,
	Pattern_2_30,
	Pattern_2_31,
	Pattern_2_32,
	Pattern_2_33,
	Pattern_2_34,
	Pattern_2_35,
	Pattern_2_36,
	Pattern_2_wall,
	Pattern_2_bossWeakPoint,
	Pattern_2_bossShield,
	NB_CALLBACKS
};

// Camera travelling handlers
enum
{
	CameraPath_i1,
	CameraPath_c1,
	CameraPath_i2,
	CameraPath_c2,
	CameraPath_c2_2,
	CameraPath_c2_boss
};

// Background handlers
enum
{
	bgHandle_default,
	bgHandle_2_2
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
