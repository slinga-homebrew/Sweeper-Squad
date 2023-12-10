#pragma once

#include <jo/jo.h>
#include "main.h"

// holds sprite and audio assets
typedef struct _assets
{
    // title screen
    int title;

    // grid
    int closed;
    int open;
    int open_select;

    // mouse cursors
    int cursors[MAX_PLAYERS];

    // mine
    int mine;
    int mine_exploded;
    int mine_first;
    int mine_wrong;

    // digits
    int digits[9];

    // flags
    int flags[MAX_TEAMS];




} ASSETS, *PASSETS;

extern ASSETS g_Assets;

// initialize polygon assets
void loadAssets(void);
//jo_palette* setGamePalette(void);
