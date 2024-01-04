#include <jo/jo.h>
#include "../main.h"
#include "../assets.h"
#include "flag.h"
#include <string.h>

FLAG g_TeamSelectFlags[MAX_TEAMS] = {0};

void initTeamSelectFlags(void)
{
    memset(g_TeamSelectFlags, 0, sizeof(g_TeamSelectFlags));

    for(unsigned int i = 0; i < COUNTOF(g_TeamSelectFlags); i++)
    {
        PFLAG flag = &g_TeamSelectFlags[i];
        flag->teamID = i;
        flag->no_players = false;
        flag->objectState = OBJECT_STATE_ACTIVE;
    }
}

void drawTeamSelectFlags(void)
{
    int flagSprite = 0;
    int x_offset = 0;

    jo_sprite_change_sprite_scale(2);

    for(int i = 0; i < MAX_TEAMS; i++)
    {
        PFLAG flag = &g_TeamSelectFlags[i];

        if(flag->objectState != OBJECT_STATE_ACTIVE)
        {
            continue;
        }

        if(flag->no_players == false)
        {
            flagSprite = g_Assets.flags2[flag->teamID];
        }
        else
        {
            flagSprite = g_Assets.mine_wrong_ts;
        }

        if(i < 6)
        {
            x_offset = -240 + (i * 42);
        }
        else
        {
            x_offset = -240 + ((i + 1) * 42);
        }

        jo_sprite_draw3D(flagSprite, x_offset, -200, 500);
    }

    //x_offset = -240 + ((6) * 42);
    //jo_sprite_draw3D(g_Assets.digits[0], x_offset, -200, 500);

    jo_sprite_change_sprite_scale(1);
}

void destroyTeamSelectFlag(unsigned int flag_index)
{
    PFLAG flag = NULL;

    if(flag_index >= MAX_TEAMS)
    {
        return;
    }

    // set the no players flag
    flag = &g_TeamSelectFlags[flag_index];
    flag->no_players = true;

    return;
}

