#pragma once

#include <SDL2/SDL_mixer.h>

#include "common.h"
#include "types.h"

class DrawingCandidates;
class Particles;

#define FPS 60

#define MAX_ENEMY 500

#define SCORE_HIT 20
#define SCORE_HIT_OP 40
#define SCORE_ABSORB 100

extern unsigned short* image_entries[NB_IMAGES];
extern unsigned short* bossImage_entries[NB_BOSS_IMAGES];
extern unsigned short* bgImage_entries[NB_BACKGROUND_IMAGES];
extern Mix_Chunk* sound_entries[NB_SOUNDS];
extern Mix_Music* music_entries[NB_MUSICS];

extern DrawingCandidates* DC;
extern Particles* G_particles;
extern int G_minX, G_maxX; // screen boundaries, changed on special occasions
extern int G_gpTimer;
extern int G_skipFrame;
extern int G_killedThisFrame[MAX_ENEMY], G_frameChainOffset, G_chainStatus, G_inChainCount, G_maxChain;
extern int G_score, G_power;
extern bool G_usingArrows;
extern bool G_fireback, G_hardMode;
extern bool G_hasFiredOnce;
extern int G_bossIntroChannel;
extern t_key* G_keys;

// Custom keys
extern t_key G_downKey, G_leftKey, G_rightKey, G_upKey, G_fireKey, G_polarityKey, G_fragmentKey, G_pauseKey;
