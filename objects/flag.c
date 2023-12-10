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
    int flagSprite = 0;
    int x_offset = 0;

    jo_sprite_change_sprite_scale(2);

    for(int i = 0; i < MAX_TEAMS; i++)
    {
        PFLAG flag = &g_TeamSelectFlags[i];


        if(flag->objectState != OBJECT_STATE_ACTIVE)
        {
            //jo_printf(2, 5, "bb state: %d", bonusBalloon->subState);
            //continue;
        }

        flagSprite = g_Assets.flags[flag->teamID];

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



    x_offset = -240 + ((6) * 42);
    jo_sprite_draw3D(g_Assets.digits[0], x_offset, -200, 500);

    jo_sprite_change_sprite_scale(1);

}

void drawTeamSelectGrid(void)
{
    int sprite = 0;
    int x_offset = 0;
    int y_offset = 0;

    for(int i = 0; i < 13; i++)
    {
        for(int j = 0; j < 13; j++)
        {
            sprite = g_Assets.open_select;

            x_offset = -240 + (i * 42);
            y_offset = -200 + 44 + (j * 28) - 8;
            jo_sprite_draw3D(sprite, x_offset, y_offset, 500);
       }
    }
}

