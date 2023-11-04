#pragma once
#include "objects.h"

typedef struct _SCORE
{
    unsigned int wave;   // last wave player was alive for
    unsigned int points; // player score
} SCORE, *PSCORE;

// This structure represents the player
typedef struct _PLAYER
{
    GAME_OBJECT;

    // 0-11, controller ID
    int playerID;

    int color;

    int numLives;

    // did player hit boost this frame?
    bool isThrusting;

    // timers (in frames)
    int invulnerabilityFrames; // how long player in invulerable

    int respawnFrames; // how long before respawning player after death

    SCORE score;

    // hack to cache controller inputs
    INPUTCACHE input;

} PLAYER, *PPLAYER;
