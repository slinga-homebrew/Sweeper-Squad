#include <jo/jo.h>
#include "main.h"
#include "assets.h"
#include "title_screen.h"
#include "objects/player.h"

// globals for menu options
int g_TitleScreenChoice = 0;
int g_TitleScreenCursorColor = 0;

typedef enum _MENU_OPTIONS
{
    TITLE_OPTION_GAME_MODE = 0,
    TITLE_OPTION_GAME_DIFFICULTY = 1,
    TITLE_OPTION_GAME_START = 2,
    TITLE_OPTION_MAX,
} MENU_OPTIONS;

static void drawTitle(void);
static void drawMenuCursor(void);
static void drawMenu(void);

// use the players as moving elements in the background
extern PLAYER g_Players[MAX_PLAYERS];

void titleScreen_init(void)
{
    // init menu options
    g_TitleScreenChoice = 0;
    //g_TitleModeChoice = 0;
    //g_TitleDifficultyChoice = 1;


    g_Game.gameMode = GAME_MODE_PRACTICE;
    g_Game.gameDifficulty = GAME_DIFFICULTY_MEDIUM;

    // randomize the cursor flag
    g_TitleScreenCursorColor = jo_random(COUNTOF(g_Assets.randomizedColors)) - 1;

    initTitlePlayers();
    return;
}

// handles input for the title screen
// only player one can control the title screen
void titleScreen_input(void)
{
    int titleScreenChoice = 0;

    if(g_Game.gameState != GAME_STATE_TITLE_SCREEN)
    {
        return;
    }

    titleScreenChoice = g_TitleScreenChoice;

    if (jo_is_pad1_key_pressed(JO_KEY_UP))
    {
        if(g_Game.input.pressedUp == false)
        {
            titleScreenChoice--;
        }
        g_Game.input.pressedUp = true;
    }
    else
    {
        g_Game.input.pressedUp = false;
    }

    if (jo_is_pad1_key_pressed(JO_KEY_DOWN))
    {
        if(g_Game.input.pressedDown == false)
        {
            titleScreenChoice++;
        }
        g_Game.input.pressedDown = true;
    }
    else
    {
        g_Game.input.pressedDown = false;
    }

    // keep title screen choice in range
    sanitizeValue(&titleScreenChoice, 0, TITLE_OPTION_MAX);

    g_TitleScreenChoice = titleScreenChoice;

    // select an option here
    if (jo_is_pad1_key_pressed(JO_KEY_START) ||
        jo_is_pad1_key_pressed(JO_KEY_A) ||
        jo_is_pad1_key_pressed(JO_KEY_C))

    {
        if(g_Game.input.pressedStart == false)
        {
            switch(titleScreenChoice)
            {
                case TITLE_OPTION_GAME_MODE:
                {
                    g_Game.gameMode++;
                    sanitizeValue(&g_Game.gameMode, 0, GAME_MODE_MAX);
                    break;
                }
                case TITLE_OPTION_GAME_DIFFICULTY:
                {
                    g_Game.gameDifficulty++;
                    sanitizeValue(&g_Game.gameDifficulty, 0, GAME_DIFFICULTY_MAX);
                    break;
                }
                case TITLE_OPTION_GAME_START:
                {
                    transitionState(GAME_STATE_TEAM_SELECT);
                    break;
                }
            }
        }
        g_Game.input.pressedStart = true;
    }
    else
    {
        g_Game.input.pressedStart = false;
    }

    return;
}

void titleScreen_update(void)
{
    if(g_Game.gameState != GAME_STATE_TITLE_SCREEN)
    {
        return;
    }

    updatePlayers();

    return;
}

void titleScreen_draw(void)
{
    if(g_Game.gameState != GAME_STATE_TITLE_SCREEN)
    {
        return;
    }

    drawPlayers();
    drawTitle();
    drawMenu();
    drawMenuCursor();

    return;
}

//
// Title screen helpers
//

// draws title image + version number
static void drawTitle(void)
{
    // title graphic
    jo_sprite_change_sprite_scale(2);
    jo_sprite_draw3D(g_Assets.title, 0, 0, TITLE_SCREEN_Z);
    jo_sprite_change_sprite_scale(1);

    // version number
    jo_printf(20, 28, "%s", VERSION);
}

// Options menu + values
static void drawMenu(void)
{
    int xPos = 0;
    int yPos = 0;

    // game mode (practice, normal, hardcore, time attack)
    xPos = -30;
    yPos = 90;

    jo_sprite_draw3D(g_Assets.mode, xPos, yPos, TITLE_MENU_Z);
    jo_sprite_draw3D(g_Assets.modes[g_Game.gameMode], xPos + 110, yPos, TITLE_MENU_Z);

    // Difficulty: (easy, medium, hard)
    yPos = 130;
    jo_sprite_draw3D(g_Assets.difficulty, xPos, yPos, TITLE_MENU_Z);
    jo_sprite_draw3D(g_Assets.difficulties[g_Game.gameDifficulty], xPos + 196, yPos, TITLE_MENU_Z);

    // Start!
    yPos = 170;
    jo_sprite_draw3D(g_Assets.start, xPos, yPos, TITLE_MENU_Z);
}

// flag used as the menu cursor
static void drawMenuCursor(void)
{
    int xPos = 0;
    int yPos = 0;

    xPos = -160;
    yPos = 90 + (g_TitleScreenChoice * 40); // vertical position varies based on selection

    jo_sprite_change_sprite_scale(2);
    jo_sprite_draw3D(g_Assets.flags2[g_TitleScreenCursorColor], xPos, yPos, TITLE_MENU_Z);
    jo_sprite_change_sprite_scale(1);
}
