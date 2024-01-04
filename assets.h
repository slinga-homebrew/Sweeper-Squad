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

    // team selection
    int open_select;
    int mine_exploded_select;

    // mouse cursors
    int cursors[MAX_PLAYERS];

    // mine
    int mine;
    int mine_exploded;
    int mine_first;
    int mine_wrong;
    int mine_wrong_ts;

    // digits
    int digits[9];

    // flags
    int flags[MAX_TEAMS];
    int flags2[MAX_TEAMS]; // flags without the cell

    // score
    int pause_c;
    int pause_d;
    int pause_p;
    int pause_r;
    int pause_s;
    int pause_t;
    int score_digits[10];

    int horizontal_seperator;
    int vertical_seperator;





} ASSETS, *PASSETS;

extern ASSETS g_Assets;

// initialize polygon assets
void loadAssets(void);
//jo_palette* setGamePalette(void);
