#include <jo/jo.h>
#include "main.h"
#include "assets.h"
#include "state.h"
#include "ssmtf_logo.h"

#define BONUS_BALLOON_Z 500

// macros for printing SSMTF logo
#define LETTER_WIDTH (30)
#define SPACE_WIDTH (15)
#define X_OFF(base, off) (base + (off*5))
#define Y_OFF(base, off) (base + (off*5))
#define LETTER_SPACING(baseX, letterCount, widthCount) ((baseX) + ((letterCount) * LETTER_WIDTH) + ((widthCount) * SPACE_WIDTH))

static void printA(int sprite, int x, int y);
static void printC(int sprite, int x, int y);
static void printD(int sprite, int x, int y);
static void printE(int sprite, int x, int y);
static void printF(int sprite, int x, int y);
static void printG(int sprite, int x, int y);
static void printH(int sprite, int x, int y);
static void printI(int sprite, int x, int y);
static void printK(int sprite, int x, int y);
static void printL(int sprite, int x, int y);
static void printM(int sprite, int x, int y);
static void printN(int sprite, int x, int y);
static void printO(int sprite, int x, int y);
static void printP(int sprite, int x, int y);
static void printR(int sprite, int x, int y);
static void printS(int sprite, int x, int y);
static void printT(int sprite, int x, int y);
static void printU(int sprite, int x, int y);
static void printY(int sprite, int x, int y);

unsigned int g_LogoTimer = 0;

// initializations for SSMTF screen
void ssmtfLogo_init(void)
{
    //g_LogoTimer
    g_LogoTimer = 0;//SSMTF_LOGO_TIMER * 10;
}

// update callback routine for SSMTF logo
void ssmtfLogo_input(void)
{
    if(g_Game.gameState != GAME_STATE_SSMTF_LOGO)
    {
        return;
    }

    //
    // skip the logo if player 1 hits start/a/b/c
    //

    if(jo_is_pad1_key_pressed(JO_KEY_START))
    {
        if(g_Game.input.pressedStart == false)
        {
            g_Game.input.pressedStart = true;
            transitionState(GAME_STATE_TITLE_SCREEN);
            return;
        }
    }
    else
    {
        g_Game.input.pressedStart = false;
    }

    // payer 1 hit a or c
    if(jo_is_pad1_key_pressed(JO_KEY_A) ||
       jo_is_pad1_key_pressed(JO_KEY_C))
    {
        if(g_Game.input.pressedAC == false)
        {
            g_Game.input.pressedAC = true;
            transitionState(GAME_STATE_TITLE_SCREEN);
            return;
        }
    }
    else
    {
        g_Game.input.pressedAC = false;
    }

    // payer 1 hit b
    if(jo_is_pad1_key_pressed(JO_KEY_B))
    {
        if(g_Game.input.pressedB == false)
        {
            g_Game.input.pressedB = true;
            transitionState(GAME_STATE_TITLE_SCREEN);
            return;
        }
    }
    else
    {
        g_Game.input.pressedB = false;
    }

    return;
}

// update callback routine for SSMTF logo
void ssmtfLogo_update(void)
{
    if(g_Game.gameState != GAME_STATE_SSMTF_LOGO)
    {
        return;
    }

    g_LogoTimer++;

    // check if the frameAnim has expired
    if(g_LogoTimer > SSMTF_LOGO_TIMER)
    {
        transitionState(GAME_STATE_TITLE_SCREEN);
        return;
    }

    return;
}

// draws Sega Saturn Multiplayer Task Force logo
void ssmtfLogo_draw(void)
{
    unsigned int frameAnim = 0;
    int sprite = g_Assets.mine_exploded;

    if(g_Game.gameState != GAME_STATE_SSMTF_LOGO)
    {
        return;
    }

    jo_sprite_change_sprite_scale(0.25);

    // TODO change to AND
    //frameAnim = getBonusBalloonFrame(g_Game.frame % 96);

    // The Sega Saturn
    //sprite = g_Assets.balloons[0][frameAnim];
    int x_offset = -212;
    int y_offset = -88;

    printT(sprite, LETTER_SPACING(x_offset, 0, 0), y_offset);
    printH(sprite, LETTER_SPACING(x_offset, 1, 0), y_offset);
    printE(sprite, LETTER_SPACING(x_offset, 2, 0), y_offset);

    printS(sprite, LETTER_SPACING(x_offset, 3, 1), y_offset);
    printE(sprite, LETTER_SPACING(x_offset, 4, 1), y_offset);
    printG(sprite, LETTER_SPACING(x_offset, 5, 1), y_offset);
    printA(sprite, LETTER_SPACING(x_offset, 6, 1), y_offset);

    printS(sprite, LETTER_SPACING(x_offset, 7, 2), y_offset);
    printA(sprite, LETTER_SPACING(x_offset, 8, 2), y_offset);
    printT(sprite, LETTER_SPACING(x_offset, 9, 2), y_offset);
    printU(sprite, LETTER_SPACING(x_offset, 10, 2), y_offset);
    printR(sprite, LETTER_SPACING(x_offset, 11, 2), y_offset);
    printN(sprite, LETTER_SPACING(x_offset, 12, 2), y_offset);

    // Multiplayer Task Force
    //sprite = g_Assets.balloons[3][frameAnim];
    y_offset += 60;
    x_offset = -310;
    printM(sprite, LETTER_SPACING(x_offset, 0, 0), y_offset);
    printU(sprite, LETTER_SPACING(x_offset, 1, 0), y_offset);
    printL(sprite, LETTER_SPACING(x_offset, 2, 0), y_offset);
    // reduce the gap between the 'L' and the 'T'
    x_offset -= 8;
    printT(sprite, LETTER_SPACING(x_offset, 3, 0), y_offset);
    printI(sprite, LETTER_SPACING(x_offset, 4, 0), y_offset);
    printP(sprite, LETTER_SPACING(x_offset, 5, 0), y_offset);
    printL(sprite, LETTER_SPACING(x_offset, 6, 0), y_offset);
    printA(sprite, LETTER_SPACING(x_offset, 7, 0), y_offset);
    printY(sprite, LETTER_SPACING(x_offset, 8, 0), y_offset);
    printE(sprite, LETTER_SPACING(x_offset, 9, 0), y_offset);
    printR(sprite, LETTER_SPACING(x_offset, 10, 0), y_offset);

    printT(sprite, LETTER_SPACING(x_offset, 11, 1), y_offset);
    printA(sprite, LETTER_SPACING(x_offset, 12, 1), y_offset);
    printS(sprite, LETTER_SPACING(x_offset, 13, 1), y_offset);
    printK(sprite, LETTER_SPACING(x_offset, 14, 1), y_offset);

    printF(sprite, LETTER_SPACING(x_offset, 15, 2), y_offset);
    printO(sprite, LETTER_SPACING(x_offset, 16, 2), y_offset);
    printR(sprite, LETTER_SPACING(x_offset, 17, 2), y_offset);
    printC(sprite, LETTER_SPACING(x_offset, 18, 2), y_offset);
    printE(sprite, LETTER_SPACING(x_offset, 19, 2), y_offset);

    // Proudly Presents
    x_offset = -230;
    y_offset += 60;
    //sprite = g_Assets.balloons[6][frameAnim];
    printP(sprite, LETTER_SPACING(x_offset, 0, 0), y_offset);
    printR(sprite, LETTER_SPACING(x_offset, 1, 0), y_offset);
    printO(sprite, LETTER_SPACING(x_offset, 2, 0), y_offset);
    printU(sprite, LETTER_SPACING(x_offset, 3, 0), y_offset);
    printD(sprite, LETTER_SPACING(x_offset, 4, 0), y_offset);
    printL(sprite, LETTER_SPACING(x_offset, 5, 0), y_offset);
    // reduce the gap between the 'L' and the 'Y'
    x_offset -= 8;
    printY(sprite, LETTER_SPACING(x_offset, 6, 0), y_offset);

    //sprite = g_Assets.balloons[7][frameAnim];
    printP(sprite, LETTER_SPACING(x_offset, 7, 1), y_offset);
    printR(sprite, LETTER_SPACING(x_offset, 8, 1), y_offset);
    printE(sprite, LETTER_SPACING(x_offset, 9, 1), y_offset);
    printS(sprite, LETTER_SPACING(x_offset, 10, 1), y_offset);
    printE(sprite, LETTER_SPACING(x_offset, 11, 1), y_offset);
    printN(sprite, LETTER_SPACING(x_offset, 12, 1), y_offset);
    printT(sprite, LETTER_SPACING(x_offset, 13, 1), y_offset);
    printS(sprite, LETTER_SPACING(x_offset, 14, 1), y_offset);

    return;
}

//
// Helper functions to draw letters for the SSMTF logo
//

static void printA(int sprite, int x, int y)
{
    jo_sprite_draw3D(sprite, X_OFF(x, 0), Y_OFF(y, 0), BONUS_BALLOON_Z);
    jo_sprite_draw3D(sprite, X_OFF(x, 0), Y_OFF(y, 1), BONUS_BALLOON_Z);
    jo_sprite_draw3D(sprite, X_OFF(x, 0), Y_OFF(y, 2), BONUS_BALLOON_Z);
    jo_sprite_draw3D(sprite, X_OFF(x, 0), Y_OFF(y, 3), BONUS_BALLOON_Z);
    jo_sprite_draw3D(sprite, X_OFF(x, 0), Y_OFF(y, 4), BONUS_BALLOON_Z);

    jo_sprite_draw3D(sprite, X_OFF(x, 1), Y_OFF(y, 0), BONUS_BALLOON_Z);
    jo_sprite_draw3D(sprite, X_OFF(x, 2), Y_OFF(y, 0), BONUS_BALLOON_Z);
    jo_sprite_draw3D(sprite, X_OFF(x, 3), Y_OFF(y, 0), BONUS_BALLOON_Z);

    jo_sprite_draw3D(sprite, X_OFF(x, 1), Y_OFF(y, 2), BONUS_BALLOON_Z);
    jo_sprite_draw3D(sprite, X_OFF(x, 2), Y_OFF(y, 2), BONUS_BALLOON_Z);
    jo_sprite_draw3D(sprite, X_OFF(x, 3), Y_OFF(y, 2), BONUS_BALLOON_Z);

    jo_sprite_draw3D(sprite, X_OFF(x, 4), Y_OFF(y, 0), BONUS_BALLOON_Z);
    jo_sprite_draw3D(sprite, X_OFF(x, 4), Y_OFF(y, 1), BONUS_BALLOON_Z);
    jo_sprite_draw3D(sprite, X_OFF(x, 4), Y_OFF(y, 2), BONUS_BALLOON_Z);
    jo_sprite_draw3D(sprite, X_OFF(x, 4), Y_OFF(y, 3), BONUS_BALLOON_Z);
    jo_sprite_draw3D(sprite, X_OFF(x, 4), Y_OFF(y, 4), BONUS_BALLOON_Z);
}

static void printC(int sprite, int x, int y)
{
    jo_sprite_draw3D(sprite, X_OFF(x, 0), Y_OFF(y, 1), BONUS_BALLOON_Z);
    jo_sprite_draw3D(sprite, X_OFF(x, 0), Y_OFF(y, 2), BONUS_BALLOON_Z);
    jo_sprite_draw3D(sprite, X_OFF(x, 0), Y_OFF(y, 3), BONUS_BALLOON_Z);

    jo_sprite_draw3D(sprite, X_OFF(x, 1), Y_OFF(y, 0), BONUS_BALLOON_Z);
    jo_sprite_draw3D(sprite, X_OFF(x, 2), Y_OFF(y, 0), BONUS_BALLOON_Z);
    jo_sprite_draw3D(sprite, X_OFF(x, 3), Y_OFF(y, 0), BONUS_BALLOON_Z);
    jo_sprite_draw3D(sprite, X_OFF(x, 4), Y_OFF(y, 0), BONUS_BALLOON_Z);

    jo_sprite_draw3D(sprite, X_OFF(x, 1), Y_OFF(y, 4), BONUS_BALLOON_Z);
    jo_sprite_draw3D(sprite, X_OFF(x, 2), Y_OFF(y, 4), BONUS_BALLOON_Z);
    jo_sprite_draw3D(sprite, X_OFF(x, 3), Y_OFF(y, 4), BONUS_BALLOON_Z);
    jo_sprite_draw3D(sprite, X_OFF(x, 4), Y_OFF(y, 4), BONUS_BALLOON_Z);
}

static void printD(int sprite, int x, int y)
{
    jo_sprite_draw3D(sprite, X_OFF(x, 0), Y_OFF(y, 0), BONUS_BALLOON_Z);
    jo_sprite_draw3D(sprite, X_OFF(x, 0), Y_OFF(y, 1), BONUS_BALLOON_Z);
    jo_sprite_draw3D(sprite, X_OFF(x, 0), Y_OFF(y, 2), BONUS_BALLOON_Z);
    jo_sprite_draw3D(sprite, X_OFF(x, 0), Y_OFF(y, 3), BONUS_BALLOON_Z);
    jo_sprite_draw3D(sprite, X_OFF(x, 0), Y_OFF(y, 4), BONUS_BALLOON_Z);

    jo_sprite_draw3D(sprite, X_OFF(x, 1), Y_OFF(y, 0), BONUS_BALLOON_Z);
    jo_sprite_draw3D(sprite, X_OFF(x, 2), Y_OFF(y, 0), BONUS_BALLOON_Z);
    jo_sprite_draw3D(sprite, X_OFF(x, 3), Y_OFF(y, 0), BONUS_BALLOON_Z);

    jo_sprite_draw3D(sprite, X_OFF(x, 4), Y_OFF(y, 1), BONUS_BALLOON_Z);
    jo_sprite_draw3D(sprite, X_OFF(x, 4), Y_OFF(y, 2), BONUS_BALLOON_Z);
    jo_sprite_draw3D(sprite, X_OFF(x, 4), Y_OFF(y, 3), BONUS_BALLOON_Z);

    jo_sprite_draw3D(sprite, X_OFF(x, 1), Y_OFF(y, 4), BONUS_BALLOON_Z);
    jo_sprite_draw3D(sprite, X_OFF(x, 2), Y_OFF(y, 4), BONUS_BALLOON_Z);
    jo_sprite_draw3D(sprite, X_OFF(x, 3), Y_OFF(y, 4), BONUS_BALLOON_Z);
}

static void printE(int sprite, int x, int y)
{
    jo_sprite_draw3D(sprite, X_OFF(x, 0), Y_OFF(y, 0), BONUS_BALLOON_Z);
    jo_sprite_draw3D(sprite, X_OFF(x, 0), Y_OFF(y, 1), BONUS_BALLOON_Z);
    jo_sprite_draw3D(sprite, X_OFF(x, 0), Y_OFF(y, 2), BONUS_BALLOON_Z);
    jo_sprite_draw3D(sprite, X_OFF(x, 0), Y_OFF(y, 3), BONUS_BALLOON_Z);
    jo_sprite_draw3D(sprite, X_OFF(x, 0), Y_OFF(y, 4), BONUS_BALLOON_Z);

    jo_sprite_draw3D(sprite, X_OFF(x, 1), Y_OFF(y, 0), BONUS_BALLOON_Z);
    jo_sprite_draw3D(sprite, X_OFF(x, 2), Y_OFF(y, 0), BONUS_BALLOON_Z);
    jo_sprite_draw3D(sprite, X_OFF(x, 3), Y_OFF(y, 0), BONUS_BALLOON_Z);
    jo_sprite_draw3D(sprite, X_OFF(x, 4), Y_OFF(y, 0), BONUS_BALLOON_Z);

    jo_sprite_draw3D(sprite, X_OFF(x, 1), Y_OFF(y, 2), BONUS_BALLOON_Z);
    jo_sprite_draw3D(sprite, X_OFF(x, 2), Y_OFF(y, 2), BONUS_BALLOON_Z);
    jo_sprite_draw3D(sprite, X_OFF(x, 3), Y_OFF(y, 2), BONUS_BALLOON_Z);

    jo_sprite_draw3D(sprite, X_OFF(x, 1), Y_OFF(y, 4), BONUS_BALLOON_Z);
    jo_sprite_draw3D(sprite, X_OFF(x, 2), Y_OFF(y, 4), BONUS_BALLOON_Z);
    jo_sprite_draw3D(sprite, X_OFF(x, 3), Y_OFF(y, 4), BONUS_BALLOON_Z);
    jo_sprite_draw3D(sprite, X_OFF(x, 4), Y_OFF(y, 4), BONUS_BALLOON_Z);
}

static void printF(int sprite, int x, int y)
{
    jo_sprite_draw3D(sprite, X_OFF(x, 0), Y_OFF(y, 0), BONUS_BALLOON_Z);
    jo_sprite_draw3D(sprite, X_OFF(x, 0), Y_OFF(y, 1), BONUS_BALLOON_Z);
    jo_sprite_draw3D(sprite, X_OFF(x, 0), Y_OFF(y, 2), BONUS_BALLOON_Z);
    jo_sprite_draw3D(sprite, X_OFF(x, 0), Y_OFF(y, 3), BONUS_BALLOON_Z);
    jo_sprite_draw3D(sprite, X_OFF(x, 0), Y_OFF(y, 4), BONUS_BALLOON_Z);

    jo_sprite_draw3D(sprite, X_OFF(x, 1), Y_OFF(y, 0), BONUS_BALLOON_Z);
    jo_sprite_draw3D(sprite, X_OFF(x, 2), Y_OFF(y, 0), BONUS_BALLOON_Z);
    jo_sprite_draw3D(sprite, X_OFF(x, 3), Y_OFF(y, 0), BONUS_BALLOON_Z);
    jo_sprite_draw3D(sprite, X_OFF(x, 4), Y_OFF(y, 0), BONUS_BALLOON_Z);

    jo_sprite_draw3D(sprite, X_OFF(x, 1), Y_OFF(y, 2), BONUS_BALLOON_Z);
    jo_sprite_draw3D(sprite, X_OFF(x, 2), Y_OFF(y, 2), BONUS_BALLOON_Z);
    jo_sprite_draw3D(sprite, X_OFF(x, 3), Y_OFF(y, 2), BONUS_BALLOON_Z);
}

static void printG(int sprite, int x, int y)
{
    jo_sprite_draw3D(sprite, X_OFF(x, 0), Y_OFF(y, 0), BONUS_BALLOON_Z);
    jo_sprite_draw3D(sprite, X_OFF(x, 0), Y_OFF(y, 1), BONUS_BALLOON_Z);
    jo_sprite_draw3D(sprite, X_OFF(x, 0), Y_OFF(y, 2), BONUS_BALLOON_Z);
    jo_sprite_draw3D(sprite, X_OFF(x, 0), Y_OFF(y, 3), BONUS_BALLOON_Z);
    jo_sprite_draw3D(sprite, X_OFF(x, 0), Y_OFF(y, 4), BONUS_BALLOON_Z);

    jo_sprite_draw3D(sprite, X_OFF(x, 1), Y_OFF(y, 0), BONUS_BALLOON_Z);
    jo_sprite_draw3D(sprite, X_OFF(x, 2), Y_OFF(y, 0), BONUS_BALLOON_Z);
    jo_sprite_draw3D(sprite, X_OFF(x, 3), Y_OFF(y, 0), BONUS_BALLOON_Z);
    jo_sprite_draw3D(sprite, X_OFF(x, 4), Y_OFF(y, 0), BONUS_BALLOON_Z);

    jo_sprite_draw3D(sprite, X_OFF(x, 2), Y_OFF(y, 2), BONUS_BALLOON_Z);
    jo_sprite_draw3D(sprite, X_OFF(x, 3), Y_OFF(y, 2), BONUS_BALLOON_Z);
    jo_sprite_draw3D(sprite, X_OFF(x, 4), Y_OFF(y, 2), BONUS_BALLOON_Z);

    jo_sprite_draw3D(sprite, X_OFF(x, 4), Y_OFF(y, 3), BONUS_BALLOON_Z);

    jo_sprite_draw3D(sprite, X_OFF(x, 1), Y_OFF(y, 4), BONUS_BALLOON_Z);
    jo_sprite_draw3D(sprite, X_OFF(x, 2), Y_OFF(y, 4), BONUS_BALLOON_Z);
    jo_sprite_draw3D(sprite, X_OFF(x, 3), Y_OFF(y, 4), BONUS_BALLOON_Z);
    jo_sprite_draw3D(sprite, X_OFF(x, 4), Y_OFF(y, 4), BONUS_BALLOON_Z);
}

static void printH(int sprite, int x, int y)
{
    jo_sprite_draw3D(sprite, X_OFF(x, 0), Y_OFF(y, 0), BONUS_BALLOON_Z);
    jo_sprite_draw3D(sprite, X_OFF(x, 0), Y_OFF(y, 1), BONUS_BALLOON_Z);
    jo_sprite_draw3D(sprite, X_OFF(x, 0), Y_OFF(y, 2), BONUS_BALLOON_Z);
    jo_sprite_draw3D(sprite, X_OFF(x, 0), Y_OFF(y, 3), BONUS_BALLOON_Z);
    jo_sprite_draw3D(sprite, X_OFF(x, 0), Y_OFF(y, 4), BONUS_BALLOON_Z);

    jo_sprite_draw3D(sprite, X_OFF(x, 1), Y_OFF(y, 2), BONUS_BALLOON_Z);
    jo_sprite_draw3D(sprite, X_OFF(x, 2), Y_OFF(y, 2), BONUS_BALLOON_Z);
    jo_sprite_draw3D(sprite, X_OFF(x, 3), Y_OFF(y, 2), BONUS_BALLOON_Z);

    jo_sprite_draw3D(sprite, X_OFF(x, 4), Y_OFF(y, 0), BONUS_BALLOON_Z);
    jo_sprite_draw3D(sprite, X_OFF(x, 4), Y_OFF(y, 1), BONUS_BALLOON_Z);
    jo_sprite_draw3D(sprite, X_OFF(x, 4), Y_OFF(y, 2), BONUS_BALLOON_Z);
    jo_sprite_draw3D(sprite, X_OFF(x, 4), Y_OFF(y, 3), BONUS_BALLOON_Z);
    jo_sprite_draw3D(sprite, X_OFF(x, 4), Y_OFF(y, 4), BONUS_BALLOON_Z);
}

static void printI(int sprite, int x, int y)
{
    jo_sprite_draw3D(sprite, X_OFF(x, 0), Y_OFF(y, 0), BONUS_BALLOON_Z);
    jo_sprite_draw3D(sprite, X_OFF(x, 1), Y_OFF(y, 0), BONUS_BALLOON_Z);
    jo_sprite_draw3D(sprite, X_OFF(x, 2), Y_OFF(y, 0), BONUS_BALLOON_Z);
    jo_sprite_draw3D(sprite, X_OFF(x, 2), Y_OFF(y, 1), BONUS_BALLOON_Z);
    jo_sprite_draw3D(sprite, X_OFF(x, 2), Y_OFF(y, 2), BONUS_BALLOON_Z);
    jo_sprite_draw3D(sprite, X_OFF(x, 2), Y_OFF(y, 3), BONUS_BALLOON_Z);
    jo_sprite_draw3D(sprite, X_OFF(x, 2), Y_OFF(y, 4), BONUS_BALLOON_Z);
    jo_sprite_draw3D(sprite, X_OFF(x, 3), Y_OFF(y, 0), BONUS_BALLOON_Z);
    jo_sprite_draw3D(sprite, X_OFF(x, 4), Y_OFF(y, 0), BONUS_BALLOON_Z);

    jo_sprite_draw3D(sprite, X_OFF(x, 0), Y_OFF(y, 4), BONUS_BALLOON_Z);
    jo_sprite_draw3D(sprite, X_OFF(x, 1), Y_OFF(y, 4), BONUS_BALLOON_Z);
    jo_sprite_draw3D(sprite, X_OFF(x, 3), Y_OFF(y, 4), BONUS_BALLOON_Z);
    jo_sprite_draw3D(sprite, X_OFF(x, 4), Y_OFF(y, 4), BONUS_BALLOON_Z);
}

static void printK(int sprite, int x, int y)
{
    jo_sprite_draw3D(sprite, X_OFF(x, 0), Y_OFF(y, 0), BONUS_BALLOON_Z);
    jo_sprite_draw3D(sprite, X_OFF(x, 0), Y_OFF(y, 1), BONUS_BALLOON_Z);
    jo_sprite_draw3D(sprite, X_OFF(x, 0), Y_OFF(y, 2), BONUS_BALLOON_Z);
    jo_sprite_draw3D(sprite, X_OFF(x, 0), Y_OFF(y, 3), BONUS_BALLOON_Z);
    jo_sprite_draw3D(sprite, X_OFF(x, 0), Y_OFF(y, 4), BONUS_BALLOON_Z);

    jo_sprite_draw3D(sprite, X_OFF(x, 1), Y_OFF(y, 2), BONUS_BALLOON_Z);
    jo_sprite_draw3D(sprite, X_OFF(x, 2), Y_OFF(y, 2), BONUS_BALLOON_Z);

    jo_sprite_draw3D(sprite, X_OFF(x, 4), Y_OFF(y, 0), BONUS_BALLOON_Z);
    jo_sprite_draw3D(sprite, X_OFF(x, 3), Y_OFF(y, 1), BONUS_BALLOON_Z);
    jo_sprite_draw3D(sprite, X_OFF(x, 3), Y_OFF(y, 3), BONUS_BALLOON_Z);
    jo_sprite_draw3D(sprite, X_OFF(x, 4), Y_OFF(y, 4), BONUS_BALLOON_Z);
}

static void printL(int sprite, int x, int y)
{
    jo_sprite_draw3D(sprite, X_OFF(x, 0), Y_OFF(y, 0), BONUS_BALLOON_Z);
    jo_sprite_draw3D(sprite, X_OFF(x, 0), Y_OFF(y, 1), BONUS_BALLOON_Z);
    jo_sprite_draw3D(sprite, X_OFF(x, 0), Y_OFF(y, 2), BONUS_BALLOON_Z);
    jo_sprite_draw3D(sprite, X_OFF(x, 0), Y_OFF(y, 3), BONUS_BALLOON_Z);
    jo_sprite_draw3D(sprite, X_OFF(x, 0), Y_OFF(y, 4), BONUS_BALLOON_Z);

    jo_sprite_draw3D(sprite, X_OFF(x, 1), Y_OFF(y, 4), BONUS_BALLOON_Z);
    jo_sprite_draw3D(sprite, X_OFF(x, 2), Y_OFF(y, 4), BONUS_BALLOON_Z);
    jo_sprite_draw3D(sprite, X_OFF(x, 3), Y_OFF(y, 4), BONUS_BALLOON_Z);
    jo_sprite_draw3D(sprite, X_OFF(x, 4), Y_OFF(y, 4), BONUS_BALLOON_Z);
}

static void printM(int sprite, int x, int y)
{
    jo_sprite_draw3D(sprite, X_OFF(x, 0), Y_OFF(y, 0), BONUS_BALLOON_Z);
    jo_sprite_draw3D(sprite, X_OFF(x, 0), Y_OFF(y, 1), BONUS_BALLOON_Z);
    jo_sprite_draw3D(sprite, X_OFF(x, 0), Y_OFF(y, 2), BONUS_BALLOON_Z);
    jo_sprite_draw3D(sprite, X_OFF(x, 0), Y_OFF(y, 3), BONUS_BALLOON_Z);
    jo_sprite_draw3D(sprite, X_OFF(x, 0), Y_OFF(y, 4), BONUS_BALLOON_Z);

    jo_sprite_draw3D(sprite, X_OFF(x, 1), Y_OFF(y, 1), BONUS_BALLOON_Z);
    jo_sprite_draw3D(sprite, X_OFF(x, 2), Y_OFF(y, 2), BONUS_BALLOON_Z);
    jo_sprite_draw3D(sprite, X_OFF(x, 3), Y_OFF(y, 1), BONUS_BALLOON_Z);

    jo_sprite_draw3D(sprite, X_OFF(x, 4), Y_OFF(y, 0), BONUS_BALLOON_Z);
    jo_sprite_draw3D(sprite, X_OFF(x, 4), Y_OFF(y, 1), BONUS_BALLOON_Z);
    jo_sprite_draw3D(sprite, X_OFF(x, 4), Y_OFF(y, 2), BONUS_BALLOON_Z);
    jo_sprite_draw3D(sprite, X_OFF(x, 4), Y_OFF(y, 3), BONUS_BALLOON_Z);
    jo_sprite_draw3D(sprite, X_OFF(x, 4), Y_OFF(y, 4), BONUS_BALLOON_Z);
}

static void printN(int sprite, int x, int y)
{
    jo_sprite_draw3D(sprite, X_OFF(x, 0), Y_OFF(y, 0), BONUS_BALLOON_Z);
    jo_sprite_draw3D(sprite, X_OFF(x, 0), Y_OFF(y, 1), BONUS_BALLOON_Z);
    jo_sprite_draw3D(sprite, X_OFF(x, 0), Y_OFF(y, 2), BONUS_BALLOON_Z);
    jo_sprite_draw3D(sprite, X_OFF(x, 0), Y_OFF(y, 3), BONUS_BALLOON_Z);
    jo_sprite_draw3D(sprite, X_OFF(x, 0), Y_OFF(y, 4), BONUS_BALLOON_Z);

    jo_sprite_draw3D(sprite, X_OFF(x, 1), Y_OFF(y, 1), BONUS_BALLOON_Z);
    jo_sprite_draw3D(sprite, X_OFF(x, 2), Y_OFF(y, 2), BONUS_BALLOON_Z);
    jo_sprite_draw3D(sprite, X_OFF(x, 3), Y_OFF(y, 3), BONUS_BALLOON_Z);

    jo_sprite_draw3D(sprite, X_OFF(x, 4), Y_OFF(y, 0), BONUS_BALLOON_Z);
    jo_sprite_draw3D(sprite, X_OFF(x, 4), Y_OFF(y, 1), BONUS_BALLOON_Z);
    jo_sprite_draw3D(sprite, X_OFF(x, 4), Y_OFF(y, 2), BONUS_BALLOON_Z);
    jo_sprite_draw3D(sprite, X_OFF(x, 4), Y_OFF(y, 3), BONUS_BALLOON_Z);
    jo_sprite_draw3D(sprite, X_OFF(x, 4), Y_OFF(y, 4), BONUS_BALLOON_Z);
}

static void printO(int sprite, int x, int y)
{
    jo_sprite_draw3D(sprite, X_OFF(x, 0), Y_OFF(y, 1), BONUS_BALLOON_Z);
    jo_sprite_draw3D(sprite, X_OFF(x, 0), Y_OFF(y, 2), BONUS_BALLOON_Z);
    jo_sprite_draw3D(sprite, X_OFF(x, 0), Y_OFF(y, 3), BONUS_BALLOON_Z);

    jo_sprite_draw3D(sprite, X_OFF(x, 1), Y_OFF(y, 0), BONUS_BALLOON_Z);
    jo_sprite_draw3D(sprite, X_OFF(x, 2), Y_OFF(y, 0), BONUS_BALLOON_Z);
    jo_sprite_draw3D(sprite, X_OFF(x, 3), Y_OFF(y, 0), BONUS_BALLOON_Z);

    jo_sprite_draw3D(sprite, X_OFF(x, 4), Y_OFF(y, 1), BONUS_BALLOON_Z);
    jo_sprite_draw3D(sprite, X_OFF(x, 4), Y_OFF(y, 2), BONUS_BALLOON_Z);
    jo_sprite_draw3D(sprite, X_OFF(x, 4), Y_OFF(y, 3), BONUS_BALLOON_Z);

    jo_sprite_draw3D(sprite, X_OFF(x, 1), Y_OFF(y, 4), BONUS_BALLOON_Z);
    jo_sprite_draw3D(sprite, X_OFF(x, 2), Y_OFF(y, 4), BONUS_BALLOON_Z);
    jo_sprite_draw3D(sprite, X_OFF(x, 3), Y_OFF(y, 4), BONUS_BALLOON_Z);
}

static void printP(int sprite, int x, int y)
{
    jo_sprite_draw3D(sprite, X_OFF(x, 0), Y_OFF(y, 0), BONUS_BALLOON_Z);
    jo_sprite_draw3D(sprite, X_OFF(x, 0), Y_OFF(y, 1), BONUS_BALLOON_Z);
    jo_sprite_draw3D(sprite, X_OFF(x, 0), Y_OFF(y, 2), BONUS_BALLOON_Z);
    jo_sprite_draw3D(sprite, X_OFF(x, 0), Y_OFF(y, 3), BONUS_BALLOON_Z);
    jo_sprite_draw3D(sprite, X_OFF(x, 0), Y_OFF(y, 4), BONUS_BALLOON_Z);

    jo_sprite_draw3D(sprite, X_OFF(x, 1), Y_OFF(y, 0), BONUS_BALLOON_Z);
    jo_sprite_draw3D(sprite, X_OFF(x, 2), Y_OFF(y, 0), BONUS_BALLOON_Z);
    jo_sprite_draw3D(sprite, X_OFF(x, 3), Y_OFF(y, 0), BONUS_BALLOON_Z);

    jo_sprite_draw3D(sprite, X_OFF(x, 1), Y_OFF(y, 2), BONUS_BALLOON_Z);
    jo_sprite_draw3D(sprite, X_OFF(x, 2), Y_OFF(y, 2), BONUS_BALLOON_Z);
    jo_sprite_draw3D(sprite, X_OFF(x, 3), Y_OFF(y, 2), BONUS_BALLOON_Z);

    jo_sprite_draw3D(sprite, X_OFF(x, 4), Y_OFF(y, 1), BONUS_BALLOON_Z);
}

static void printR(int sprite, int x, int y)
{
    jo_sprite_draw3D(sprite, X_OFF(x, 0), Y_OFF(y, 0), BONUS_BALLOON_Z);
    jo_sprite_draw3D(sprite, X_OFF(x, 0), Y_OFF(y, 1), BONUS_BALLOON_Z);
    jo_sprite_draw3D(sprite, X_OFF(x, 0), Y_OFF(y, 2), BONUS_BALLOON_Z);
    jo_sprite_draw3D(sprite, X_OFF(x, 0), Y_OFF(y, 3), BONUS_BALLOON_Z);
    jo_sprite_draw3D(sprite, X_OFF(x, 0), Y_OFF(y, 4), BONUS_BALLOON_Z);

    jo_sprite_draw3D(sprite, X_OFF(x, 1), Y_OFF(y, 0), BONUS_BALLOON_Z);
    jo_sprite_draw3D(sprite, X_OFF(x, 2), Y_OFF(y, 0), BONUS_BALLOON_Z);
    jo_sprite_draw3D(sprite, X_OFF(x, 3), Y_OFF(y, 0), BONUS_BALLOON_Z);

    jo_sprite_draw3D(sprite, X_OFF(x, 1), Y_OFF(y, 2), BONUS_BALLOON_Z);
    jo_sprite_draw3D(sprite, X_OFF(x, 2), Y_OFF(y, 2), BONUS_BALLOON_Z);
    jo_sprite_draw3D(sprite, X_OFF(x, 3), Y_OFF(y, 2), BONUS_BALLOON_Z);

    jo_sprite_draw3D(sprite, X_OFF(x, 4), Y_OFF(y, 1), BONUS_BALLOON_Z);
    jo_sprite_draw3D(sprite, X_OFF(x, 4), Y_OFF(y, 3), BONUS_BALLOON_Z);
    jo_sprite_draw3D(sprite, X_OFF(x, 4), Y_OFF(y, 4), BONUS_BALLOON_Z);
}

static void printS(int sprite, int x, int y)
{
    jo_sprite_draw3D(sprite, X_OFF(x, 0), Y_OFF(y, 0), BONUS_BALLOON_Z);
    jo_sprite_draw3D(sprite, X_OFF(x, 1), Y_OFF(y, 0), BONUS_BALLOON_Z);
    jo_sprite_draw3D(sprite, X_OFF(x, 2), Y_OFF(y, 0), BONUS_BALLOON_Z);
    jo_sprite_draw3D(sprite, X_OFF(x, 3), Y_OFF(y, 0), BONUS_BALLOON_Z);
    jo_sprite_draw3D(sprite, X_OFF(x, 4), Y_OFF(y, 0), BONUS_BALLOON_Z);

    jo_sprite_draw3D(sprite, X_OFF(x, 0), Y_OFF(y, 1), BONUS_BALLOON_Z);

    jo_sprite_draw3D(sprite, X_OFF(x, 0), Y_OFF(y, 2), BONUS_BALLOON_Z);
    jo_sprite_draw3D(sprite, X_OFF(x, 1), Y_OFF(y, 2), BONUS_BALLOON_Z);
    jo_sprite_draw3D(sprite, X_OFF(x, 2), Y_OFF(y, 2), BONUS_BALLOON_Z);
    jo_sprite_draw3D(sprite, X_OFF(x, 3), Y_OFF(y, 2), BONUS_BALLOON_Z);
    jo_sprite_draw3D(sprite, X_OFF(x, 4), Y_OFF(y, 2), BONUS_BALLOON_Z);

    jo_sprite_draw3D(sprite, X_OFF(x, 4), Y_OFF(y, 3), BONUS_BALLOON_Z);

    jo_sprite_draw3D(sprite, X_OFF(x, 0), Y_OFF(y, 4), BONUS_BALLOON_Z);
    jo_sprite_draw3D(sprite, X_OFF(x, 1), Y_OFF(y, 4), BONUS_BALLOON_Z);
    jo_sprite_draw3D(sprite, X_OFF(x, 2), Y_OFF(y, 4), BONUS_BALLOON_Z);
    jo_sprite_draw3D(sprite, X_OFF(x, 3), Y_OFF(y, 4), BONUS_BALLOON_Z);
    jo_sprite_draw3D(sprite, X_OFF(x, 4), Y_OFF(y, 4), BONUS_BALLOON_Z);
}

static void printT(int sprite, int x, int y)
{
    jo_sprite_draw3D(sprite, X_OFF(x, 0), Y_OFF(y, 0), BONUS_BALLOON_Z);
    jo_sprite_draw3D(sprite, X_OFF(x, 1), Y_OFF(y, 0), BONUS_BALLOON_Z);
    jo_sprite_draw3D(sprite, X_OFF(x, 2), Y_OFF(y, 0), BONUS_BALLOON_Z);
    jo_sprite_draw3D(sprite, X_OFF(x, 2), Y_OFF(y, 1), BONUS_BALLOON_Z);
    jo_sprite_draw3D(sprite, X_OFF(x, 2), Y_OFF(y, 2), BONUS_BALLOON_Z);
    jo_sprite_draw3D(sprite, X_OFF(x, 2), Y_OFF(y, 3), BONUS_BALLOON_Z);
    jo_sprite_draw3D(sprite, X_OFF(x, 2), Y_OFF(y, 4), BONUS_BALLOON_Z);
    jo_sprite_draw3D(sprite, X_OFF(x, 3), Y_OFF(y, 0), BONUS_BALLOON_Z);
    jo_sprite_draw3D(sprite, X_OFF(x, 4), Y_OFF(y, 0), BONUS_BALLOON_Z);
}

static void printU(int sprite, int x, int y)
{
    jo_sprite_draw3D(sprite, X_OFF(x, 0), Y_OFF(y, 0), BONUS_BALLOON_Z);
    jo_sprite_draw3D(sprite, X_OFF(x, 0), Y_OFF(y, 1), BONUS_BALLOON_Z);
    jo_sprite_draw3D(sprite, X_OFF(x, 0), Y_OFF(y, 2), BONUS_BALLOON_Z);
    jo_sprite_draw3D(sprite, X_OFF(x, 0), Y_OFF(y, 3), BONUS_BALLOON_Z);
    jo_sprite_draw3D(sprite, X_OFF(x, 0), Y_OFF(y, 4), BONUS_BALLOON_Z);

    jo_sprite_draw3D(sprite, X_OFF(x, 1), Y_OFF(y, 4), BONUS_BALLOON_Z);
    jo_sprite_draw3D(sprite, X_OFF(x, 2), Y_OFF(y, 4), BONUS_BALLOON_Z);
    jo_sprite_draw3D(sprite, X_OFF(x, 3), Y_OFF(y, 4), BONUS_BALLOON_Z);

    jo_sprite_draw3D(sprite, X_OFF(x, 4), Y_OFF(y, 0), BONUS_BALLOON_Z);
    jo_sprite_draw3D(sprite, X_OFF(x, 4), Y_OFF(y, 1), BONUS_BALLOON_Z);
    jo_sprite_draw3D(sprite, X_OFF(x, 4), Y_OFF(y, 2), BONUS_BALLOON_Z);
    jo_sprite_draw3D(sprite, X_OFF(x, 4), Y_OFF(y, 3), BONUS_BALLOON_Z);
    jo_sprite_draw3D(sprite, X_OFF(x, 4), Y_OFF(y, 4), BONUS_BALLOON_Z);
}

static void printY(int sprite, int x, int y)
{
    jo_sprite_draw3D(sprite, X_OFF(x, 0), Y_OFF(y, 0), BONUS_BALLOON_Z);
    jo_sprite_draw3D(sprite, X_OFF(x, 0), Y_OFF(y, 1), BONUS_BALLOON_Z);

    jo_sprite_draw3D(sprite, X_OFF(x, 1), Y_OFF(y, 2), BONUS_BALLOON_Z);

    jo_sprite_draw3D(sprite, X_OFF(x, 2), Y_OFF(y, 3), BONUS_BALLOON_Z);
    jo_sprite_draw3D(sprite, X_OFF(x, 2), Y_OFF(y, 4), BONUS_BALLOON_Z);

    jo_sprite_draw3D(sprite, X_OFF(x, 3), Y_OFF(y, 2), BONUS_BALLOON_Z);

    jo_sprite_draw3D(sprite, X_OFF(x, 4), Y_OFF(y, 0), BONUS_BALLOON_Z);
    jo_sprite_draw3D(sprite, X_OFF(x, 4), Y_OFF(y, 1), BONUS_BALLOON_Z);
}
