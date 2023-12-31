#include <jo/jo.h>
#include "main.h"
#include "objects/flag.h"
#include "objects/player.h"
#include "assets.h"

extern PLAYER g_Players[MAX_PLAYERS];


#define TRANSITION_TIMER (6 * FRAMES_IN_SECONDS) // how long to wait after transitioning

int g_StartGameFrames = 0;
bool g_TeamSelectPressedStart = false;

bool validateTeams(void);
void updateTeamSelectPlayers(void);

void drawTeamSelectGrid(void)
{
    int sprite = 0;
    int x_offset = 0;
    int y_offset = 0;

    for(int i = 0; i < 13; i++)
    {
        for(int j = 0; j < 13; j++)
        {
            if(i == 6 && g_TeamSelectPressedStart)
            {
                sprite = g_Assets.mine_exploded_select;
            }
            else
            {
                sprite = g_Assets.open_select;
            }

            x_offset = -240 + (i * 42);
            y_offset = -200 + 44 + (j * 28) - 8;
            jo_sprite_draw3D(sprite, x_offset, y_offset, 500);
       }
    }
}

// initializations for SSMTF screen
void teamSelect_init(void)
{
    initPlayers();
    initTeamSelectFlags();

    g_TeamSelectPressedStart = false;
    g_StartGameFrames = TRANSITION_TIMER;
}

// handles input for the title screen menu
// only player one can control the title screen
void teamSelect_input(void)
{
    int teamSelectChoice = 0;

    if(g_Game.gameState != GAME_STATE_TEAM_SELECT)
    {
        return;
    }

    // already pressed start, nothing to do
    if(g_TeamSelectPressedStart == true)
    {
        return;
    }

    // did player 1 hit start
    if(jo_is_pad1_key_pressed(JO_KEY_START))
    {
        if(g_Game.input.pressedStart == false)
        {
            bool result;

            g_Game.input.pressedStart = true;

            result = validateTeams();
            if(result != true)
            {
                // todo: play bad noise
                //jo_audio_play_sound(&g_Assets.skidPCM);
                return;
            }

            g_TeamSelectPressedStart = true;
            g_StartGameFrames = TRANSITION_TIMER;
            return;
        }
    }
    else
    {
        g_Game.input.pressedStart = false;
    }

    // did player 1 hit B
    if(jo_is_pad1_key_pressed(JO_KEY_B))
    {
        if(g_Game.input.pressedB == false)
        {
            // return to the title screen
            g_Game.input.pressedB = true;
            transitionState(GAME_STATE_TITLE_SCREEN);
            return;
        }
    }
    else
    {
        g_Game.input.pressedB = false;
    }

    for(int i = 0; i < MAX_PLAYERS; i++)
    {
        PPLAYER player = &g_Players[i];

        teamSelectChoice = player->teamSelectChoice;

        // did the player hit a direction key
        if (jo_is_input_key_down(player->playerID, JO_KEY_LEFT))
        {
            if(player->input.pressedLeft == false)
            {
                teamSelectChoice--;
            }
            player->input.pressedLeft = true;
        }
        else
        {
            player->input.pressedLeft = false;
        }

        if (jo_is_input_key_down(player->playerID, JO_KEY_RIGHT))
        {
            if(player->input.pressedRight == false)
            {
                teamSelectChoice++;
            }
            player->input.pressedRight = true;
        }
        else
        {
            player->input.pressedRight = false;
        }

        // validate the title screen choices
        if(teamSelectChoice < 0)
        {
            teamSelectChoice = 12;
        }
        if(teamSelectChoice > 12)
        {
            teamSelectChoice = 0;
        }
        player->teamSelectChoice = teamSelectChoice;
    }

    return;
}

void teamSelect_update(void)
{
    if(g_Game.gameState != GAME_STATE_TEAM_SELECT)
    {
        return;
    }

    if(g_TeamSelectPressedStart == true)
    {
        g_StartGameFrames--;

        if(g_StartGameFrames <= 0)
        {
            transitionState(GAME_STATE_GAMEPLAY);
        }
    }

    updateTeamSelectPlayers();
    updatePlayers();
}

// draws the team select menu
void teamSelect_draw(void)
{
    if(g_Game.gameState != GAME_STATE_TEAM_SELECT)
    {
        return;
    }

    drawTeamSelectFlags();
    drawTeamSelectGrid();
    drawPlayers();

    return;
}

// story at least 1 team
// versus requires at least 2 teams
bool validateTeams(void)
{
    int numTeams = 0;
    int teams[MAX_TEAMS] = {0};
    int minTeams = 0;

    for(int i = 0; i < MAX_PLAYERS; i++)
    {
        PPLAYER player = &g_Players[i];
        int teamID = 0;

        if(player->teamSelectChoice == 6)
        {
            // TODO: fix this
            continue;
            //teamID = 5;
            //player->teamSelectChoice = 5;

        }
        else if(player->teamSelectChoice > 6)
        {
            teamID = player->teamSelectChoice - 1;
        }
        else
        {
            teamID = player->teamSelectChoice;
        }

        teams[teamID] = teams[teamID] + 1;
    }

    for(int i = 0; i < MAX_TEAMS; i++)
    {
        if(teams[i] > 0)
        {
            numTeams++;
        }
    }

    // different game types require different number of teams
//    if(g_Game.gameType == GAME_TYPE_STORY)
    {
        minTeams = 1;
    }
//    else if(g_Game.gameType == GAME_TYPE_VERSUS)
    {
  //      minTeams = 2;
    }

    if(numTeams < minTeams)
    {
        return false;
    }

    // destroy the flag of empty teams
    for(int i = 0; i < MAX_TEAMS; i++)
    {
        if(teams[i] == 0)
        {
            destroyTeamSelectFlag(i);
        }
    }

    // drop the players aren't in
    for(int i = 0; i < MAX_PLAYERS; i++)
    {
        PPLAYER player = &g_Players[i];

        if(player->teamSelectChoice == 6)
        {

            explodePlayer(player);
            continue;
        }

        if(player->teamSelectChoice > 6)
        {
            player->teamSelectChoice--;
        }
    }

    return true;
}

void updateTeamSelectPlayers(void)
{
    if(g_TeamSelectPressedStart == true)
    {
        return;
    }

    for(int i = 0; i < MAX_PLAYERS; i++)
    {
        PPLAYER player = &g_Players[i];
        int playerX = 0;
        int playerY = 0;

        playerX = -240 + (player->teamSelectChoice * 42);
        playerY = -200 + 8 + ((i + 1) * 28);

        player->curPos.x = toFIXED(playerX);
        player->curPos.y = toFIXED(playerY);

        if(i >= 6)
        {
            player->curPos.y += toFIXED(28);
        }
    }

    return;
}
