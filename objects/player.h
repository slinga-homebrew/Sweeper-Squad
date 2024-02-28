#pragma once
#include "object.h"

#define NOT_PLAYING 0
#define PLAYING 1

typedef enum _PLAYER_STATE
{
    PLAYER_STATE_ACTIVE = 0,
    PLAYER_STATE_EXPLODING,
    PLAYER_STATE_EXPLODED,
    PLAYER_STATE_LOGO, // used for the cursor in the logo
    PLAYER_STATE_TITLE, // cursors in the background of the title screen
    PLAYER_STATE_DEAD,
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

    jo_fixed size;
    jo_fixed ds;

    int angle;
    int da;

    int frameCount;

    // 0-11, controller ID
    int playerID;

    // 0 if not playing, 1 if playing
    int isPlaying;

    // what team the player is on
    int teamSelectChoice;
    int teamID;

    int numLives;

    // timers (in frames)
    int invulnerabilityFrames; // how long player in invulerable

    int respawnFrames; // how long before respawning player after death

    int crackChoice; // style of cracked screen graphic

    SCORE score;

    // hack to cache controller inputs
    INPUTCACHE input;

    // TODO: not needed?
    bool moveHorizontal;
    bool moveVertical;

} PLAYER, *PPLAYER;

void initPlayers(void);
void getPlayersInput(void);
void updatePlayers(void);
void drawPlayers(void);
void explodePlayer(PPLAYER player, bool showExplosion, bool spreadExplosion);

void initTitlePlayers(void);
void resetPlayerScores(void);
void spawnPlayers(void);
void cacheInputDirectionPlayer(PPLAYER player, bool* up, bool* down, bool* left, bool *right);



