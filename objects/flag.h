#pragma once
#include "object.h"

// This structure represents the land mine flag
typedef struct _FLAG
{
    GAME_OBJECT;

    // 0-11
    int teamID;

} FLAG, *PFLAG;


//void initPlayers(void);
//void getPlayersInput(void);
//void updatePlayers(void);
//void drawPlayers(void);


void initTeamSelectFlags(void);
void drawTeamSelectFlags(void);
