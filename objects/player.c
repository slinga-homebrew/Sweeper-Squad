#include <jo/jo.h>
#include "../main.h"
#include "../assets.h"
#include "player.h"

PLAYER g_Players[MAX_PLAYERS] = {0};

void drawPlayerSprite(PPLAYER player);

void initPlayers(void)
{
    PPLAYER player = NULL;

    memset(g_Players, 0, sizeof(g_Players));

    for(unsigned int i = 0; i < COUNTOF(g_Players); i++)
    {
        player = &g_Players[i];

        //memset(&player->curPos, 0, sizeof(FIXED_POSITION));
        //memset(&player->oldPos, 0, sizeof(FIXED_POSITION));
        //memset(&player->startPos, 0, sizeof(FIXED_POSITION));

        player->playerID = i;

        player->objectState = OBJECT_STATE_ACTIVE;

        //player->animation = PLAYER_ANIMATION_IDLE;
        //player->hasSplashed = false;

        player->curPos.x = toFIXED((i*49.23) - 320 + 49.23);
        player->curPos.x = toFIXED(jo_random(640) - 320);

        // the player is initially not on a team
        player->teamSelectChoice = 6;

        //player->numBalloons = 2;

        //player->invulnerableTimer = 0; // make players invulnerable on spawn
    }
}


void getPlayersInput(void)
{
    return;
}

void updatePlayers(void)
{
    return;
}

void drawPlayers(void)
{
    for(unsigned int i = 0; i <  COUNTOF(g_Players); i++)
    {
        PPLAYER player = &g_Players[i];

        if(player->objectState == OBJECT_STATE_INACTIVE)
        {
            continue;
        }

        /*
        // make invulnerable players flicker
        if(player->invulnerableTimer > 0)
        {
            if((player->invulnerableTimer % 5) == 0)
            {
                continue;
            }
        }
        */

        /*
        if(player->dirRight == true)
        {
            jo_sprite_enable_horizontal_flip();
        }
        */

        //jo_printf(13, 12+i, "Player %d %d          active", i, MAX_PLAYERS);

        drawPlayerSprite(player);

    }
}

void drawPlayerSprite(PPLAYER player)
{
    int playerSprite = 0;

    /*
    if(player->dirRight == true)
    {
        jo_sprite_enable_horizontal_flip();
    }

    playerSprite = getPlayerSprite(player);
    */

    //playerSprite = g_Assets.cursors[player->playerID];

    if(player->playerID >= 0 && player->playerID < 12)
    {
       playerSprite = g_Assets.cursors[player->playerID];
       jo_sprite_draw3D(playerSprite, toINT(player->curPos.x), toINT(player->curPos.y), PLAYER_Z);
    }

    //jo_sprite_change_sprite_scale(3);


    //jo_sprite_draw3D(g_Assets.fish[4][2], toINT(player->curPos.x), toINT(player->curPos.y), FISH_Z);
    //jo_sprite_change_sprite_scale(1);

    /*
    // draw another sprite if the player has wrapped the screen
    // todo: check if the player is out of position before drawing
    if(player->curPos.x  + toFIXED(8.0) > SCREEN_RIGHT)
    {
        jo_sprite_draw3D(playerSprite, toINT(player->curPos.x - SCREEN_WIDTH), toINT(player->curPos.y), PLAYER_Z);
    }

    // draw another sprite if the player has wrapped the screen
    if(player->curPos.x - toFIXED(8.0) < SCREEN_LEFT)
    {
        jo_sprite_draw3D(playerSprite, toINT(player->curPos.x + SCREEN_WIDTH), toINT(player->curPos.y), PLAYER_Z);
    }

    if(player->dirRight == true)
    {
        jo_sprite_disable_horizontal_flip();
    }
    */
}
