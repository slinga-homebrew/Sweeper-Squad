#pragma once
#include "object.h"

// This structure represents the land mine flag
typedef struct _FLAG
{
    GAME_OBJECT;

    // 0-11
    int teamID;

    bool no_players; // flag used in team select to show no players are on the team

} FLAG, *PFLAG;

void initTeamSelectFlags(void);
void drawTeamSelectFlags(void);
void destroyTeamSelectFlag(unsigned int flag_index);
