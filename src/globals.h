#pragma once

#include <SDL2/SDL_mixer.h>

#include "common.h"
#include "types.h"
#include "helpers/Constants.hpp"

class DrawingCandidates;
class Particles;

extern DrawingCandidates* DC;
extern Particles* G_particles;
extern int G_minX, G_maxX; // screen boundaries, changed on special occasions
extern int G_gpTimer;
extern int G_skipFrame;
extern int G_killedThisFrame[Constants::MAX_ENEMY], G_frameChainOffset, G_chainStatus, G_inChainCount, G_maxChain;
extern int G_score, G_power;
extern bool G_usingArrows;
extern bool G_fireback, G_hardMode;
extern bool G_hasFiredOnce;
extern int G_bossIntroChannel;
extern const t_key* G_keys;

// Custom keys
extern t_key G_downKey, G_leftKey, G_rightKey, G_upKey, G_fireKey, G_polarityKey, G_fragmentKey, G_pauseKey;
