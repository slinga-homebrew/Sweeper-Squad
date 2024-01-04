#include <jo/jo.h>
#include <stdlib.h>
#include "main.h"
#include "assets.h"
#include "objects/player.h"

extern PLAYER g_Players[MAX_PLAYERS];


typedef enum _PAUSE_OPTIONS
{
    PAUSE_OPTIONS_RESUME_OR_RESTART = 0,
    PAUSE_OPTIONS_QUIT = 1,
    PAUSE_OPTION_MAX,
} PAUSE_OPTIONS;

static int sortByScore(const void * player1, const void * player2);
static int sortByLives(const void * player1, const void * player2);
static void validateScores(void);

static void drawPauseLines(void);
static void drawPauseLegend(void);
static void drawPauseScore(void);
static void drawPauseScoreShip(/*jo_3d_mesh* mesh, int xPos, int yPos, int color*/);
static void drawPauseMenuCursor(void);

static void checkForPausePress(void);
static void checkForPauseMenu(void);

int g_PauseCursor = 0;
PAUSE_OPTIONS g_PauseChoice = 0;

//
// Pause callbacks
//

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

    /*
    drawPauseMenuCursor();
    */

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
    //index = jo_random(COUNTOF(g_Assets.randomizedColors)) - 1;
    //g_PauseCursorColor = g_Assets.randomizedColors[index];
    g_PauseCursor = g_Assets.flags[0];

    g_PauseChoice = 0;

    g_Game.isPaused = true;

    // TODO:
    //playCDTrack(track);
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
            pauseGame(0);
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

    // keep pause choice in range
    if(pauseChoice < 0)
    {
        pauseChoice = PAUSE_OPTION_MAX - 1;
    }
    else if(pauseChoice >= PAUSE_OPTION_MAX)
    {
        pauseChoice = 0;
    }

    g_PauseChoice = pauseChoice;

    if (jo_is_pad1_key_pressed(JO_KEY_START))
    {
        if(g_Game.input.pressedStart == false)
        {
            switch(pauseChoice)
            {
                case PAUSE_OPTIONS_RESUME_OR_RESTART:
                {
                    // the leftmost option can restart the game on game over
                    // or unpause if paused depending on the game state
                    if(g_Game.isGameOver == true)
                    {
                        transitionState(GAME_STATE_GAMEPLAY);
                        break;
                    }
                    else
                    {
                        // simply unpause
                        g_Game.isPaused = false;
                        break;
                    }
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

        if(g_Players[i].score.points > 1000)
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

    // TODO: check for not playing and lower them

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

// player ships in the score
static void drawPauseScoreShip(/*jo_3d_mesh* mesh, int xPos, int yPos, int color*/)
{
    /*
    jo_3d_push_matrix();
    {
        jo_3d_camera_look_at(&g_Game.camera);
        jo_3d_set_mesh_color(mesh, color); // todo
        jo_3d_translate_matrix(xPos + 12, yPos - 2, 0);
        jo_3d_rotate_matrix(0, 0, 90);
        jo_3d_mesh_draw(mesh);
    }
    jo_3d_pop_matrix();
    */
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
    int nonPlayerCount = 0;
    int color = 0;
    int xPos = 0;
    int yPos = 0;
    int xScale = 1;
    int yScale = 1;
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
        unsigned char digit1 = 0;
        unsigned char digit2 = 0;
        unsigned char digit3 = 0;
        unsigned int lives = 0;
        unsigned int waves = 0;
        unsigned int score = 0;
        PPLAYER player = NULL;
        int shipColor = 0;

        player = &sortedPlayers[i];

        yPos += 28;

        // rank
        xPos = -196;

        digit1 = ((i+1) / 10);
        digit2 = ((i+1) % 10);

        if(digit1 != 0)
        {
            jo_sprite_draw3D(g_Assets.score_digits[digit1], xPos + (letterSpacing*0), yPos, PLAYER_Z);
        }
        jo_sprite_draw3D(g_Assets.score_digits[digit2], xPos + (letterSpacing*1), yPos, PLAYER_Z);

        // player ship
        //mesh = g_Assets.ships[i];

        xPos += (letterSpacing * 2) + 48;


        // TODO: should be team sprite
        playerSprite = g_Assets.flags2[player->playerID];
        jo_sprite_draw3D(playerSprite, xPos, yPos, PLAYER_Z);

        xPos += 64;


        //drawPauseScoreShip(mesh, xPos, yPos, shipColor);
        playerSprite = g_Assets.cursors[player->playerID];
        jo_sprite_draw3D(playerSprite, xPos-4, yPos, PLAYER_Z);


        // lives
        xPos += 48;

        lives = player->score.deaths;

        digit1 = (lives / 10);
        digit2 = (lives % 10);

        jo_sprite_draw3D(g_Assets.score_digits[digit1], xPos + (letterSpacing*0), yPos, PLAYER_Z);
        jo_sprite_draw3D(g_Assets.score_digits[digit2], xPos + (letterSpacing*1), yPos, PLAYER_Z);

        // wave
        waves = player->score.points;

        xPos += 48 + (letterSpacing*2);
        digit1 = (waves / 100);
        digit2 = ((waves - ((waves/100) * 100))/10);
        digit3 = (waves % 10);

        jo_sprite_draw3D(g_Assets.score_digits[digit1], xPos + (letterSpacing*0), yPos, PLAYER_Z);
        jo_sprite_draw3D(g_Assets.score_digits[digit2], xPos + (letterSpacing*1), yPos, PLAYER_Z);
        jo_sprite_draw3D(g_Assets.score_digits[digit3], xPos + (letterSpacing*2), yPos, PLAYER_Z);

        // points
        score = player->score.team_points;

        xPos += 48 + (letterSpacing*3);
        digit1 = (score / 100);
        digit2 = ((score - ((score/100) * 100))/10);
        digit3 = (score % 10);

        jo_sprite_draw3D(g_Assets.score_digits[digit1], xPos + (letterSpacing*0), yPos, PLAYER_Z);
        jo_sprite_draw3D(g_Assets.score_digits[digit2], xPos + (letterSpacing*1), yPos, PLAYER_Z);
        jo_sprite_draw3D(g_Assets.score_digits[digit3], xPos + (letterSpacing*2), yPos, PLAYER_Z);
    }

}

// cursor ship
static void drawPauseMenuCursor(void)
{
    /*
    int color = g_PauseCursorColor;
    int xPos = -72 + (g_PauseChoice * 128);
    int yPos = 91;

    jo_3d_mesh* mesh = g_Assets.cursorShip;

    jo_3d_push_matrix();
    {
        jo_3d_camera_look_at(&g_Game.camera);
        jo_3d_set_mesh_color(mesh, color);
        jo_3d_translate_matrix(xPos, yPos, 0);
        jo_3d_rotate_matrix(0, 0, 90);
        jo_3d_mesh_draw(mesh);
    }
    jo_3d_pop_matrix();

    return;
    */
}
