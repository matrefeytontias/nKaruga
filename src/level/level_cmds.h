#pragma once

#include "helpers/Constants.hpp"

// Level streams

// x, y, HP, image ID, callback ID, polarity, has rotation ?, fireback amount, type ?
#define enemy(x, y, HP, iID, cbID, p, hR, f, type) x, y, HP, idtoi<LUTs::BaseImageId>(iID), cbID, p, hR, f, type

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

template <typename LUTid>
constexpr int idtoi(LUTid id)
{
	return static_cast<int>(id);
}

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
#define cmd_useBackground(id, x, y, scrollScale, displayScale, handleID) LVLSTR_CMD, 7, LVLSTR_BACKGROUND, idtoi<LUTs::BgImageId>(id), x, y, scrollScale, displayScale, handleID
// Empties the background rendering stack
#define cmd_resetBackgrounds LVLSTR_CMD, 1, LVLSTR_RESETBG
// Plays the chapter's music
#define cmd_playMusic(mainM, loopM) LVLSTR_CMD, 3, LVLSTR_MUSIC, idtoi<LUTs::MusicId>(mainM), idtoi<LUTs::MusicId>(loopM)
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
