#include <jo/jo.h>
#include "main.h"
#include "assets.h"
#include "title_screen.h"

void titleScreen_init(void)
{
    return;
}

void titleScreen_input(void)
{
    if(g_Game.gameState != GAME_STATE_TITLE_SCREEN)
    {
        return;
    }


    // select an option here
    if (jo_is_pad1_key_pressed(JO_KEY_START))
        //jo_is_pad1_key_pressed(JO_KEY_A) ||
        //jo_is_pad1_key_pressed(JO_KEY_C))
    {
        g_Game.input.pressedStart = true;
        g_Game.input.pressedAC = true;

        transitionState(GAME_STATE_GAMEPLAY);
        return;
    }
    else
    {
        g_Game.input.pressedStart = false;
        g_Game.input.pressedAC = false;
    }

    return;
}

void titleScreen_update(void)
{
    if(g_Game.gameState != GAME_STATE_TITLE_SCREEN)
    {
        return;
    }

    return;
}

void titleScreen_draw(void)
{
    if(g_Game.gameState != GAME_STATE_TITLE_SCREEN)
    {
        return;
    }

    jo_sprite_change_sprite_scale(2);
    jo_sprite_draw3D(g_Assets.title, 0, 0, 500);
    jo_sprite_change_sprite_scale(1);


    return;
}
