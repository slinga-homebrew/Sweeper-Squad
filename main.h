#pragma once

#include <jo/jo.h>
#include "state.h"
#include "constants.h"
#include "util.h"

#define VERSION "v0.9"

// supported game types
typedef enum _GAME_MODE
{
    GAME_MODE_PRACTICE = 0, // unlimited lives
    GAME_MODE_NORMAL = 1, // 3 lives
    GAME_MODE_HARDCORE = 2, // 1 life
    GAME_MODE_TIME_ATTACK = 3, // only one team can move at a time.
    GAME_MODE_MAX,
} GAME_MODE;

// supported game difficulty
typedef enum _GAME_DIFFICULTY
{
    GAME_DIFFICULTY_EASY = 0,
    GAME_DIFFICULTY_MEDIUM = 1,
    GAME_DIFFICULTY_HARD = 2,
    GAME_DIFFICULTY_MAX,
} GAME_DIFFICULTY;

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

    int prev_mouse_x;
    int prev_mouse_y;

} INPUTCACHE, *PINPUTCACHE;

// represents game state
typedef struct _GAME
{
    // current game state
    GAME_STATE gameState;

    // practice, normal, hardcore, or time attack
    GAME_MODE gameMode;

    // easy, medium, hard
    // affects how many mines are in the game
    GAME_DIFFICULTY gameDifficulty;

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
extern GAME g_Game;

void cacheInputDirection(bool* up, bool* down, bool* left, bool *right);

