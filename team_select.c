#include <jo/jo.h>
#include "main.h"
#include "objects/flag.h"
#include "objects/player.h"
#include "assets.h"

//extern BONUS_BALLOON g_TeamSelectBonusBalloons[MAX_TEAMS];
extern PLAYER g_Players[MAX_PLAYERS];

// TODO: fix
#define TRANSITION_TIMER (6 * FRAMES_IN_SECONDS) // how long to wait after transitioning
//#define TRANSITION_TIMER (0)

int g_StartGameFrames = TRANSITION_TIMER;
bool g_TeamSelectPressedStart = false;

bool validateTeams(void);
void updateTeamSelectPlayers(void);

// initializations for SSMTF screen
void teamSelect_init(void)
{
    //initStars();
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

    drawPlayers();
    //drawWater();
    //drawStars();
    drawTeamSelectFlags();
    //drawSplashes();

    // stop any audio that's playing
    //jo_audio_stop_cd();

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

    // pop the balloons of empty teams
    for(int i = 0; i < MAX_TEAMS; i++)
    {
        if(teams[i] == 0)
        {
            //popBonusBalloon(&g_TeamSelectBonusBalloons[i], NULL);
        }
    }

    // drop the players aren't in
    for(int i = 0; i < MAX_PLAYERS; i++)
    {
        PPLAYER player = &g_Players[i];

        if(player->teamSelectChoice == 6)
        {
            //cryPlayer(player);
            // explode player?
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

        playerX = -240 + (player->teamSelectChoice * 40);
        playerY = -200 + ((i + 1) * 31);

        player->curPos.x = toFIXED(playerX);
        player->curPos.y = toFIXED(playerY);

        if(player->teamSelectChoice < 6)
        {
            //setPlayerDir(player, RIGHT);
        }
        else
        {
            //setPlayerDir(player, LEFT);
        }
    }

    return;
}
