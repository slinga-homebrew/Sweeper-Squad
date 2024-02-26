/*
**   Sweeper Squad - 12 player Minesweeper clone by Slinga
*/

/*
** Jo Sega Saturn Engine
** Copyright (c) 2012-2017, Johannes Fetz (johannesfetz@gmail.com)
** All rights reserved.
**
** Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are met:
**     * Redistributions of source code must retain the above copyright
**       notice, this list of conditions and the following disclaimer.
**     * Redistributions in binary form must reproduce the above copyright
**       notice, this list of conditions and the following disclaimer in the
**       documentation and/or other materials provided with the distribution.
**     * Neither the name of the Johannes Fetz nor the
**       names of its contributors may be used to endorse or promote products
**       derived from this software without specific prior written permission.
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
** ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
** WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
** DISCLAIMED. IN NO EVENT SHALL Johannes Fetz BE LIABLE FOR ANY
** DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
** (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
** LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
** ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
** SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include <jo/jo.h>
#include "main.h"
#include "assets.h"
#include "gameplay.h"
#include "pause.h"
#include "ssmtf_logo.h"
#include "team_select.h"
#include "title_screen.h"
#include "objects/player.h"

extern PLAYER g_Players[MAX_PLAYERS];


GAME g_Game = {0};
ASSETS g_Assets = {0};
extern Uint16 TotalPolygons;

extern Uint16 VDP2_CRAOFB;

// global callbacks, not tied to a specific game state
void abcStart_callback(void);
void debug_input(void);
void debug_draw(void);

// sets up callback and starts game loop
void jo_main(void)
{
    jo_core_init(JO_COLOR_Black);

    //
    // load\init game assets
    //

    #if defined(JO_480i)
        slZoomNbg0(toFIXED(0.5), toFIXED(0.5));
        VDP2_CRAOFB = 0x0010 | (VDP2_CRAOFB & 0xFF0F);
    #endif

    // disable pre-clipping for perf (Thanks Fafling!)
    __jo_sprite_attributes.effect |= (1 << 11);

    // load sprites + PCM audio
    loadAssets();

    //
    // game callbacks
    //

    jo_core_add_callback(ssmtfLogo_input);
    jo_core_add_callback(ssmtfLogo_update);
    jo_core_add_callback(ssmtfLogo_draw);

    jo_core_add_callback(titleScreen_input);
    jo_core_add_callback(titleScreen_update);
    jo_core_add_callback(titleScreen_draw);

    jo_core_add_callback(teamSelect_input);
    jo_core_add_callback(teamSelect_update);
    jo_core_add_callback(teamSelect_draw);

    jo_core_add_callback(gameplay_input);
    jo_core_add_callback(gameplay_update);
    jo_core_add_callback(gameplay_draw);

    jo_core_add_callback(pause_input);
    jo_core_add_callback(pause_draw);

    jo_core_add_callback(debug_input);
    jo_core_add_callback(debug_draw);

    // ABC + start handler
    jo_core_set_restart_game_callback(abcStart_callback);

    // transition to first game state
    jo_set_default_background_color(JO_COLOR_Gray);

    // BUG:JO_COLOR_INDEX_Black doesn't seem to workfor me
    // something with 480i + 8bpp palettes
    // https://github.com/johannes-fetz/joengine/issues/72
    jo_set_printf_color_index(JO_COLOR_INDEX_Yellow);

    // TODO
    g_Game.debug = 1;

    transitionState(GAME_STATE_SSMTF_LOGO);
    //transitionState(GAME_STATE_TEAM_SELECT);
    //transitionState(GAME_STATE_GAMEPLAY);
    //transitionState(GAME_STATE_TITLE_SCREEN);

    // game loop
    jo_core_run();
}

//
// Global callbacks (not tied to specific game state)
//

// returns to title screen if player one presses ABC+Start
void abcStart_callback(void)
{
    if(g_Game.gameState == GAME_STATE_UNINITIALIZED)
    {
        // ignore callback if not yet initialized
        return;
    }

    // don't do anything if already at the title screen
    if(g_Game.gameState != GAME_STATE_TITLE_SCREEN)
    {
        g_Game.input.pressedStart = true;
        g_Game.input.pressedAC = true;
        g_Game.input.pressedB = true;

        transitionState(GAME_STATE_TITLE_SCREEN);

        return;
    }

    return;
}

// check if player 1 pressed Z to change the debug level
// this can occur at any time
void debug_input(void)
{
    // player 1 press start
    if (jo_is_pad1_key_pressed(JO_KEY_Z))
    {
        if(g_Game.input.pressedZ == false)
        {
            g_Game.debug++;

            if(g_Game.debug > MAX_DEBUG_LEVEL)
            {
                g_Game.debug = 0;
                jo_clear_screen();
            }
        }
        g_Game.input.pressedZ = true;
    }
    else
    {
        g_Game.input.pressedZ = false;
    }
}

// display FPS and sprite count
void debug_draw(void)
{
    if(g_Game.debug == 0)
    {
        return;
    }

    //jo_fixed_point_time();
    //slPrintFX(delta_time, slLocate(2,25));
    //jo_printf(0, 25, "Frame:");
    //jo_printf(0, 26, "Total Sprites: %d", TotalPolygons); // says polygons, but is actually sprite count
    //jo_printf(0, 27, "Num Sprites: %d", jo_sprite_count());
    //jo_printf(0, 28, "Sprites: %d", jo_sprite_usage_percent());

}

void cacheInputDirection(bool* up, bool* down, bool* left, bool *right)
{
    jo_gamepad_type gamepad_type = JoNotConnectedGamepad;
    int mouse_x = 0;
    int mouse_y = 0;
    int mouse_dx = 0;
    int mouse_dy = 0;

    *up = FALSE;
    *down = FALSE;
    *left = FALSE;
    *right = FALSE;

    // Saturn supports two mouse types
    gamepad_type = jo_get_input_type(0);
    if(gamepad_type != JoRegularMouse && gamepad_type != JoShuttleMouse)
    {
        // no mouse in controller port, treat like regular controller
        *up = jo_is_pad1_key_pressed(JO_KEY_UP);
        *down = jo_is_pad1_key_pressed(JO_KEY_DOWN);
        *left = jo_is_pad1_key_pressed(JO_KEY_LEFT);
        *right = jo_is_pad1_key_pressed(JO_KEY_RIGHT);
        return;
    }

    // player 1 is using a mouse
    mouse_x = jo_get_mouse_pos_x(0);
    mouse_y = jo_get_mouse_pos_y(0);

    mouse_dx = g_Game.input.prev_mouse_x - mouse_x;
    mouse_dy = g_Game.input.prev_mouse_y - mouse_y;

    jo_printf(10, 10, "%d %d %d %d            ", mouse_x, mouse_y, mouse_dx, mouse_dy);

    if(mouse_dx > 1)
    {
        *left = TRUE;
    }
    else if(mouse_dx < -1)
    {
        *right = TRUE;
    }

    if(mouse_dy > 1)
    {
        *up = TRUE;
    }
    else if(mouse_dy < -1)
    {
        *down = TRUE;
    }

    g_Game.input.prev_mouse_x = mouse_x;
    g_Game.input.prev_mouse_y = mouse_y;
}
