#pragma once

#include <jo/jo.h>
#include "state.h"


#define GAME_VERSION "0.00"
#define MAX_DEBUG_LEVEL (1)

// supported game types
typedef enum _GAME_TYPE
{
    GAME_TYPE_COOP = 0,
    GAME_TYPE_VERSUS,
    GAME_TYPE_MAX,
} GAME_TYPE;

// number of lives per game
typedef enum _GAME_LIVES
{
    GAME_LIVES_1,
    GAME_LIVES_3,
    GAME_LIVES_6,
    GAME_LIVES_9,
    GAME_LIVES_MAX,
} GAME_LIVES;

// records whether or not an input has been pressed that frame
typedef struct _inputCache{

    bool pressedUp;
    bool pressedDown;
    bool pressedLeft;
    bool pressedRight;
    bool pressedAC;
    bool pressedB;
    bool pressedX;
    bool pressedY;
    bool pressedZ;
    bool pressedStart;
    bool pressedLT;
    bool pressedRT;
} INPUTCACHE, *PINPUTCACHE;

// represents game state
typedef struct _GAME
{
    // current game state
    GAME_STATE gameState;

    // coop or versus
    GAME_TYPE gameType;

    // debug level
    int debug;

    // number of lives (1, 3, 6, 9)
    int numLives;

    // is the game is paused
    bool isPaused;

    // is the game finished?
    bool isGameOver;

    // how many frames since all players were dead
    int gameOverFrames;

    // hack to cache controller inputs
    // used for menus by player 1
    INPUTCACHE input;

} GAME, *PGAME;

// globals
//extern PLAYER g_Players[MAX_PLAYERS];
extern GAME g_Game;
