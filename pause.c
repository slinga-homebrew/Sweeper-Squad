#include <jo/jo.h>
#include <stdlib.h>
#include <string.h>
#include "main.h"
#include "assets.h"
#include "objects/player.h"
#include "audio.h"

extern PLAYER g_Players[MAX_PLAYERS];

int g_PauseChoice = 0;


typedef enum _PAUSE_OPTIONS
{
    PAUSE_OPTIONS_RESUME = 0,
    PAUSE_OPTIONS_RESTART = 1,
    PAUSE_OPTIONS_QUIT = 2,
    PAUSE_OPTION_MAX,
} PAUSE_OPTIONS;

static int sortByScore(const void * player1, const void * player2);
//static int sortByTime(const void * player1, const void * player2);
static void validateScores(void);

static void drawPauseLines(void);
static void drawPauseLegend(void);
static void drawPauseScore(void);
static void drawPauseScoreShip(/*jo_3d_mesh* mesh, int xPos, int yPos, int color*/);
static void drawPauseMenuCursor(void);
static void drawPauseMenu(void);

static void checkForPausePress(void);
static void checkForPauseMenu(void);

int g_PauseCursor = 0;
//PAUSE_OPTIONS g_PauseChoice = 0;

//
// Pause callbacks
//

void convertNumberToDigits(int number, unsigned char* hunds, unsigned char* tens, unsigned char* ones)
{
    if(number == 0)
    {
        *hunds = 0;
        *tens = 0;
        *ones = 0;

        return;
    }

    *hunds = (number / 100);
    *tens = ((number - ((number/100) * 100))/10);
    *ones = (number % 10);

    return;
}

// input for handling pause
void pause_input(void)
{
    if(g_Game.gameState != GAME_STATE_GAMEPLAY)
    {
        return;
    }

    if(g_Game.isPaused == false)
    {
        // only check for pause press if the game is unpaused
        checkForPausePress();
    }
    else
    {
        // only check for pause menu presses if the game is paused
        checkForPauseMenu();
    }

    return;
}

// draw the current score while the game is paused
void pause_draw(void)
{
    if(g_Game.gameState != GAME_STATE_GAMEPLAY)
    {
        return;
    }

    if(g_Game.isPaused == false)
    {
        return;
    }

    //
    // score
    //

    drawPauseScore();
    drawPauseLines();
    drawPauseLegend();
    drawPauseMenu();
    drawPauseMenuCursor();

    return;
}

// pause the game
void pauseGame(int track)
{
    int index = 0;

    g_Game.input.pressedLeft = true;
    g_Game.input.pressedRight = true;

    // TODO: implement random flag
    // randomly choose a color for the cursor ship
    g_PauseCursor = jo_random(COUNTOF(g_Assets.randomizedColors)) - 1;
    //g_PauseCursorColor = g_Assets.randomizedColors[index];
    //g_PauseCursor = g_Assets.flags[0];

    g_PauseChoice = 0;

    g_Game.isPaused = true;

    // TODO:
    playCDTrack(track);
}

//
// Pause private functions
//

// check if player 1 paused the game
static void checkForPausePress(void)
{
    // player 1 press start
    if (jo_is_pad1_key_pressed(JO_KEY_START))
    {
        if(g_Game.input.pressedStart == false)
        {
            //TODO:
            pauseGame(PAUSE_TRACK);
        }
        g_Game.input.pressedStart = true;
    }
    else
    {
        g_Game.input.pressedStart = false;
    }
}

// pause menu options
static void checkForPauseMenu(void)
{
    int pauseChoice = g_PauseChoice;

    if (jo_is_pad1_key_pressed(JO_KEY_LEFT))
    {
        if(g_Game.input.pressedLeft == false)
        {
            pauseChoice--;
        }
        g_Game.input.pressedLeft = true;
    }
    else
    {
        g_Game.input.pressedLeft = false;
    }

    if (jo_is_pad1_key_pressed(JO_KEY_RIGHT))
    {
        if(g_Game.input.pressedRight == false)
        {
            pauseChoice++;
        }
        g_Game.input.pressedRight = true;
    }
    else
    {
        g_Game.input.pressedRight = false;
    }

    // keep pause screen choice in range
    sanitizeValue(&pauseChoice, 0, PAUSE_OPTION_MAX);

    g_PauseChoice = pauseChoice;

    if (jo_is_pad1_key_pressed(JO_KEY_START))
    {
        if(g_Game.input.pressedStart == false)
        {
            switch(pauseChoice)
            {
                case PAUSE_OPTIONS_RESUME:
                {
                    // simply unpause
                    g_Game.isPaused = false;
                    break;
                }
                case PAUSE_OPTIONS_RESTART:
                {
                    // start a new game without going to title or team select
                    transitionState(GAME_STATE_GAMEPLAY);
                    break;

                }
                case PAUSE_OPTIONS_QUIT:
                {
                    // rightmost option always quits to the title screen
                    transitionState(GAME_STATE_TITLE_SCREEN);
                    break;
                }
            }
        }

        g_Game.input.pressedStart = true;
        return;
    }
    else
    {
        g_Game.input.pressedStart = false;
    }

    return;
}


// normalize scores before displaying in game over or pause screen
static void validateScores(void)
{
    int teams[MAX_PLAYERS] = {0};


    for(int i = 0; i < MAX_PLAYERS; i++)
    {
        // normalize score data
        if(g_Players[i].score.deaths > 100)
        {
            g_Players[i].score.deaths = 99;
        }

        if(g_Players[i].score.points > 1000)
        {
            g_Players[i].score.points = 999;
        }

        teams[g_Players[i].teamID] += g_Players[i].score.points;
    }

    for(int i = 0; i < MAX_PLAYERS; i++)
    {
        g_Players[i].score.team_points = teams[g_Players[i].teamID];

        // normalize score data
        if(g_Players[i].score.team_points > 1000)
        {
            g_Players[i].score.team_points = 999;
        }
    }

    return;
}

// sort players by score for coop mode
static int sortByScore(const void * player1, const void * player2)
{
    PPLAYER a = (PPLAYER)player1;
    PPLAYER b = (PPLAYER)player2;

    // teams have equal score
    if(b->score.team_points == a->score.team_points)
    {
        // are they on different teams?
        if(b->teamID != a->teamID)
        {
            // tie-breaker: lower player ID wins
            if(b->teamID < a->teamID)
            {
                return 1;
            }
            else
            {
                return -1;
            }
        }

        // fall through, these are players on the same team
        // we want to sort them
    }

    // sort by team points
    if(b->score.team_points != a->score.team_points)
    {
        if(b->score.team_points > a->score.team_points)
        {
            return 1;
        }
        else
        {
            return -1;
        }
    }

    // TODO: figure out team id check as we want to list all winning teams before losing teams

    // sort by score
    if(b->score.points != a->score.points)
    {
        if(b->score.points > a->score.points)
        {
            return 1;
        }
        else
        {
            return -1;
        }
    }

    // sort by lives (more is worse)
    if(b->score.deaths != a->score.deaths)
    {
        if(b->score.deaths > a->score.deaths)
        {
            return -1;
        }
        else
        {
            return 1;
        }
    }

    // tie-breaker: lower player ID wins
    if(b->playerID < a->playerID)
    {
        return 1;
    }
    else
    {
        return -1;
    }

    return 0;
}

// vertical and horizontal lines framing the scores
static void drawPauseLines(void)
{
    int xPos = 0;
    int yPos = 0;

    // horizontal line between Rank/Team and Player Score/Team Score

    xPos = 136;
    yPos = -124;
    jo_sprite_draw3D(g_Assets.vertical_seperator, xPos, yPos, PLAYER_Z);
    jo_sprite_draw3D(g_Assets.vertical_seperator, xPos, yPos + 136, PLAYER_Z);
    jo_sprite_draw3D(g_Assets.vertical_seperator, xPos, yPos + (136*2) - 44, PLAYER_Z);

    xPos = -150;
    yPos = -124;
    jo_sprite_draw3D(g_Assets.vertical_seperator, xPos, yPos, PLAYER_Z);
    jo_sprite_draw3D(g_Assets.vertical_seperator, xPos, yPos + 136, PLAYER_Z);
    jo_sprite_draw3D(g_Assets.vertical_seperator, xPos, yPos + (136*2) - 44, PLAYER_Z);


    // vertical line between legend and scores
    xPos = -184 + 30 + 16 - 4;
    yPos = -180 - 28 + 16 + 16 + 8;

    jo_sprite_draw3D(g_Assets.horizontal_seperator, xPos, yPos, PLAYER_Z);
    jo_sprite_draw3D(g_Assets.horizontal_seperator, xPos + 144, yPos, PLAYER_Z);
    jo_sprite_draw3D(g_Assets.horizontal_seperator, xPos + 144*2, yPos, PLAYER_Z);


}

// legend for the score chart
static void drawPauseLegend(void)
{
    int xPos = -184;
    int yPos = -180 - 28 + 16 + 8;
    int xDelta = 60;

    jo_sprite_change_sprite_scale(2);


    // 'R'ank
    jo_sprite_draw3D(g_Assets.pause_r, xPos, yPos, PLAYER_Z);
    xPos += xDelta + 10 - 4;

    // 'T'eam //TODO
    jo_sprite_draw3D(g_Assets.pause_t, xPos, yPos, PLAYER_Z);
    xPos += xDelta;

    // 'C'haracter
    jo_sprite_draw3D(g_Assets.pause_c, xPos, yPos, PLAYER_Z);
    xPos += xDelta;

    // 'D'eaths
    jo_sprite_draw3D(g_Assets.pause_d, xPos, yPos, PLAYER_Z);
    xPos += xDelta + 16;

    // 'P'layer 'S'core
    jo_sprite_draw3D(g_Assets.pause_p, xPos, yPos, PLAYER_Z);
    xPos += 18;
    jo_sprite_draw3D(g_Assets.pause_s, xPos, yPos, PLAYER_Z);
    xPos += xDelta + 16;

    // 'T'eam 'S'core
    jo_sprite_draw3D(g_Assets.pause_t, xPos, yPos, PLAYER_Z);
    xPos += 19;
    jo_sprite_draw3D(g_Assets.pause_s, xPos, yPos, PLAYER_Z);
    xPos += xDelta;

    jo_sprite_change_sprite_scale(1);
}

// pause scores
static void drawPauseScore(void)
{
    PLAYER sortedPlayers[MAX_PLAYERS] = {0};
    int playerSprite = 0;
    int xPos = 0;
    int yPos = 0;
    int letterSpacing = 15;

    validateScores();
    memcpy(sortedPlayers, g_Players, sizeof(sortedPlayers));

    // rank by score
    qsort(sortedPlayers, COUNTOF(sortedPlayers), sizeof(PLAYER), sortByScore);

    //color = g_Game.hudColor;
    yPos = -180;

    // loop through all the sorted players displaying the score
    for(unsigned int i = 0; i < COUNTOF(sortedPlayers); i++)
    {
        unsigned int deaths = 0;
        unsigned int points = 0;
        unsigned int team_points = 0;
        unsigned char digit1 = 0;
        unsigned char digit2 = 0;
        unsigned char digit3 = 0;
        PPLAYER player = NULL;
        int yellow = 7;

        player = &sortedPlayers[i];

        yPos += 28;

        // rank
        xPos = -196;

        digit1 = ((i+1) / 10);
        digit2 = ((i+1) % 10);

        if(digit1 != 0)
        {
            jo_sprite_draw3D(g_Assets.scores[yellow][digit1], xPos + (letterSpacing*0), yPos, PLAYER_Z);
        }
        jo_sprite_draw3D(g_Assets.scores[yellow][digit2], xPos + (letterSpacing*1), yPos, PLAYER_Z);
        xPos += (letterSpacing * 2) + 48;


        // team flag
        playerSprite = g_Assets.flags2[g_Assets.randomizedColors[player->teamID]];
        jo_sprite_draw3D(playerSprite, xPos, yPos, PLAYER_Z);
        xPos += 64;

        // player cursor
        playerSprite = g_Assets.cursors[player->playerID];
        jo_sprite_draw3D(playerSprite, xPos-4, yPos, PLAYER_Z);
        xPos += 48;

        // deaths
        deaths = player->score.deaths;

        digit1 = (deaths / 10);
        digit2 = (deaths % 10);

        jo_sprite_draw3D(g_Assets.scores[yellow][digit1], xPos + (letterSpacing*0), yPos, PLAYER_Z);
        jo_sprite_draw3D(g_Assets.scores[yellow][digit2], xPos + (letterSpacing*1), yPos, PLAYER_Z);
        xPos += 48 + (letterSpacing*2);

        // points
        points = player->score.points;

        digit1 = (points / 100);
        digit2 = ((points - ((points/100) * 100))/10);
        digit3 = (points % 10);

        jo_sprite_draw3D(g_Assets.scores[yellow][digit1], xPos + (letterSpacing*0), yPos, PLAYER_Z);
        jo_sprite_draw3D(g_Assets.scores[yellow][digit2], xPos + (letterSpacing*1), yPos, PLAYER_Z);
        jo_sprite_draw3D(g_Assets.scores[yellow][digit3], xPos + (letterSpacing*2), yPos, PLAYER_Z);

        // team points
        team_points = player->score.team_points;

        xPos += 48 + (letterSpacing*3);
        digit1 = (team_points / 100);
        digit2 = ((team_points - ((team_points/100) * 100))/10);
        digit3 = (team_points % 10);

        jo_sprite_draw3D(g_Assets.scores[yellow][digit1], xPos + (letterSpacing*0), yPos, PLAYER_Z);
        jo_sprite_draw3D(g_Assets.scores[yellow][digit2], xPos + (letterSpacing*1), yPos, PLAYER_Z);
        jo_sprite_draw3D(g_Assets.scores[yellow][digit3], xPos + (letterSpacing*2), yPos, PLAYER_Z);
    }
}

// Options menu + values
static void drawPauseMenu(void)
{
    int xPos = 0;
    int yPos = 0;

    // game mode (practice, normal, hardcore, time attack)
    xPos = -160;
    yPos = 184;

    // resume
    jo_sprite_draw3D(g_Assets.resume, xPos, yPos, TITLE_MENU_Z);

    // retry
    xPos = 0;
    //yPos = 184;
    jo_sprite_draw3D(g_Assets.retry, xPos, yPos, TITLE_MENU_Z);

    // exit
    xPos = 160;
    //yPos = 190;
    jo_sprite_draw3D(g_Assets.exit, xPos, yPos, TITLE_MENU_Z);
}

// The cursor is a random flag
static void drawPauseMenuCursor(void)
{
    int xPos = 0;
    int yPos = 0;

    xPos = -210 + (g_PauseChoice * 160); // horizontal position varies based on selection
    yPos = 184;

    jo_sprite_draw3D(g_Assets.flags2[g_PauseCursor], xPos, yPos, TITLE_MENU_Z);
}
