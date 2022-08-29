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

// Level streams
// x, y, HP, image ID, callback ID, polarity, has rotation ?, fireback amount, type ?
#define enemy(x, y, HP, iID, cbID, p, hR, f, type) x, y, HP, static_cast<int>(iID), cbID, p, hR, f, type

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

// Special values
#define LVLSTR_CHAPTEREND -3
#define LVLSTR_END -2
#define LVLSTR_CMD -1

// Commands
enum
{
	LVLSTR_NEWWAVE,
	LVLSTR_NEWCAMERA,
	LVLSTR_WAIT,
	LVLSTR_WAITABS,
	LVLSTR_WAITCAMERA,
	LVLSTR_SKIP,
	LVLSTR_SKIPABS,
	LVLSTR_REPEATABS,
	LVLSTR_KILLED,
	LVLSTR_BACKGROUND,
	LVLSTR_RESETBG,
	LVLSTR_MUSIC,
	LVLSTR_REINIT,
	LVLSTR_CHAPTER,
	LVLSTR_JOINT,
	LVLSTR_BOSS,
	LVLSTR_BKPT
};

// Starts a new wave
#define cmd_newWave LVLSTR_CMD, 1, LVLSTR_NEWWAVE
// Loads a new camera path
#define cmd_newCameraPath(n) LVLSTR_CMD, 2, LVLSTR_NEWCAMERA, n
// Waits a given amount of frames
#define cmd_wait(x) LVLSTR_CMD, 2, LVLSTR_WAIT, x
// Waits until G_gpTimer has a certain value
#define cmd_waitAbs(x) LVLSTR_CMD, 2, LVLSTR_WAITABS, x
// Waits until the ABSOLUTE camera reaches this point in the level
#define cmd_waitCamera(x, y) LVLSTR_CMD, 3, LVLSTR_WAITCAMERA, x, y
// Skips the nb next commands if G_waveTimer is greater than time
#define cmd_skip(time, nb) LVLSTR_CMD, 3, LVLSTR_SKIP, time, nb
// Skips the nb next commands if G_gpTimer is greater than time
#define cmd_skipAbs(time, nb) LVLSTR_CMD, 3, LVLSTR_SKIPABS, time, nb
// Go back nb commands before the current position (not including this one) if G_gpTimer is lower than time
#define cmd_repeatAbs(time, nb) LVLSTR_CMD, 3, LVLSTR_REPEATABS, time, nb
// Waits for all enemies to be killed (includes props)
#define cmd_killed LVLSTR_CMD, 1, LVLSTR_KILLED
// Pushes the given background to the background rendering stack
#define cmd_useBackground(id, x, y, scrollScale, displayScale, handleID) LVLSTR_CMD, 7, LVLSTR_BACKGROUND, static_cast<int>(id), x, y, scrollScale, displayScale, handleID
// Empties the background rendering stack
#define cmd_resetBackgrounds LVLSTR_CMD, 1, LVLSTR_RESETBG
// Plays the chapter's music
#define cmd_playMusic(mainM, loopM) LVLSTR_CMD, 3, LVLSTR_MUSIC, static_cast<int>(mainM), static_cast<int>(loopM)
// Loads a new chapter
#define cmd_newChapter(n) LVLSTR_CMD, 2, LVLSTR_REINIT, n
// Starts a new chapter (call it after its introduction is done)
#define cmd_startChapter() LVLSTR_CMD, 1, LVLSTR_CHAPTER
// Joints an enemy to another enemy (given their waveIndex numbers) at the point (x, y)
// If the enemy is joint-dependant, it dies when the anchor enemy dies.
#define cmd_joint(which, to, x, y, jointDependant) LVLSTR_CMD, 6, LVLSTR_JOINT, which, to, x, y, jointDependant
// Breakpoint
#define cmd_bkpt LVLSTR_CMD, 1, LVLSTR_BKPT
// Fight the boss of chapter n with music playing
#define cmd_fightBoss(n) LVLSTR_CMD, 2, LVLSTR_BOSS, n

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

// Camera relations
enum
{
	CAMREL_NONE,
	CAMREL_ABSOLUTE,
	CAMREL_RELATIVE
};

// Background handlers
enum
{
	bgHandle_default,
	bgHandle_2_2
};
