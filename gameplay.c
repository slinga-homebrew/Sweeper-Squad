#include <jo/jo.h>
#include "main.h"
#include "gameplay.h"
#include "objects/grid.h"
#include "objects/player.h"


extern PLAYER g_Players[MAX_PLAYERS];

bool g_RoundOver = false;

int countActiveTeams()
{
    int teamCount[12] = {0}; // number of players on each team
    int activeTeams = 0;

    for(unsigned int i = 0; i < COUNTOF(g_Players); i++)
    {
        PPLAYER player = &g_Players[i];

        if(player->objectState == OBJECT_STATE_INACTIVE)
        {
            continue;
        }

        teamCount[player->teamSelectChoice] = teamCount[player->teamSelectChoice] + 1;
    }

    for(unsigned int j = 0; j < COUNTOF(teamCount); j++)
    {
        if(teamCount[j] > 0)
        {
            activeTeams++;
        }
    }

    return activeTeams;
}

bool isRoundOver(void)
{
    /*
    if(g_Game.gameType == GAME_TYPE_STORY)
    {
        if(isBonusLevel() == true)
        {
            // bonus round
            // all bonus balloons popped
            for(unsigned int i = 0; i < COUNTOF(g_BonusBalloons); i++)
            {
                PBONUS_BALLOON balloon = &g_BonusBalloons[i];

                if(balloon->objectState == OBJECT_STATE_ACTIVE)
                {
                    return false;
                }
            }
            return true;
        }
        else
        {
            // regular
            // - all players dead
            // - all enemies dead
            return false;
        }
    }
    else
    {
        // versus
        // only one team standing
        if(countActiveTeams() < 2)
        {
            return true;
        }
        return false;
    }
    */

    return false;
}

void gameplay_init(void)
{

    // init grid?
    //getPlayerSpawnLocations(g_Players, false);

    initGrid();

    initPlayers(); // todo: this should be called in team select, not here
                   // we should only be calling spawn location



    g_Game.isPaused = false;
    //g_RoundOver = false;
    //g_Game.dramaticDeathCounter = 0;
    //g_BonusBalloonsPopped = 0;
}

void gameplay_input(void)
{
    if(g_Game.gameState != GAME_STATE_GAMEPLAY)
    {
        return;
    }

    if(g_Game.isPaused == true)
    {
        return;
    }

    getPlayersInput();
}


void gameplay_update(void)
{
    if(g_Game.gameState != GAME_STATE_GAMEPLAY)
    {
        return;
    }

    updatePlayers();

}

void gameplay_draw(void)
{
    if(g_Game.gameState != GAME_STATE_GAMEPLAY)
    {
        return;
    }

    drawGrid();
    drawPlayers();
}


