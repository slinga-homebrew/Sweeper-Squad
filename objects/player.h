#pragma once
#include "object.h"

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

    // what team the player is one
    int teamSelectChoice;

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

    bool moveHorizontal;
    bool moveVertical;

} PLAYER, *PPLAYER;

void initPlayers(void);
void getPlayersInput(void);
void updatePlayers(void);
void drawPlayers(void);
//void openSquare(PPLAYER player);

