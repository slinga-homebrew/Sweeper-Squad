#include <jo/jo.h>
#include "../main.h"
#include "../assets.h"
#include "flag.h"

FLAG g_TeamSelectFlags[MAX_TEAMS] = {0};

void initTeamSelectFlags(void)
{
    memset(g_TeamSelectFlags, 0, sizeof(g_TeamSelectFlags));

    for(unsigned int i = 0; i < COUNTOF(g_TeamSelectFlags); i++)
    {
        PFLAG flag = &g_TeamSelectFlags[i];
        flag->teamID = i;
        flag->objectState = OBJECT_STATE_ACTIVE;
    }
}

void drawTeamSelectFlags(void)
{
    for(int i = 0; i < MAX_TEAMS; i++)
    {
        PFLAG flag = &g_TeamSelectFlags[i];
        int flagSprite = 0;
        int x_offset = 0;

        if(flag->objectState != OBJECT_STATE_ACTIVE)
        {
            //jo_printf(2, 5, "bb state: %d", bonusBalloon->subState);
            //continue;
        }

        flagSprite = g_Assets.flags[flag->teamID];

        if(i < 6)
        {
            x_offset = -240 + (i * 40);
        }
        else
        {
            x_offset = -240 + ((i + 1) * 40);
        }

        jo_sprite_change_sprite_scale(2);
        {
            jo_sprite_draw3D(flagSprite, x_offset, -200, 500);
        }
        jo_sprite_change_sprite_scale(1);
    }
}
