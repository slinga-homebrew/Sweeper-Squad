#pragma once
#include "object.h"

// Explosion after player triggers mine
typedef struct _EXPLOSION
{
    GAME_OBJECT;

    // 0-11
    int teamID;

    int frameCount;

} EXPLOSION, *PEXPLOSION;

void initExplosions(void);
void updateExplosions(void);
void drawExplosions(void);

void spawnExplosion(PPLAYER player);

