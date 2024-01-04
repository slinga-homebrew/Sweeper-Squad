#pragma once
#include "object.h"

#define EXPLODE_FRAME_COUNT (500)

typedef enum _PLAYER_STATE
{
    PLAYER_STATE_ACTIVE = 0,
    PLAYER_STATE_EXPLODING = 1,
} PLAYER_STATE;

typedef struct _SCORE
{
    unsigned int deaths; // last wave player was alive for
    unsigned int points; // number of boxes opened
    unsigned int team_points; // points of entire team


} SCORE, *PSCORE;

// This structure represents the player
typedef struct _PLAYER
{
    GAME_OBJECT;
    PLAYER_STATE subState;

    int frameCount;

    // 0-11, controller ID
    int playerID;

    // what team the player is one
    int teamSelectChoice;

    int numLives;

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
void explodePlayer(PPLAYER player);


