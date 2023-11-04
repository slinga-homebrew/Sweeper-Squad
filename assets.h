#pragma once

#include <jo/jo.h>
#include "main.h"

// holds sprite and audio assets
typedef struct _assets
{
    // title screen
    int title;

    // mouse cursors

    // bombs

    // digits

    // flags




} ASSETS, *PASSETS;

// initialize polygon assets
void loadSpriteAssets();

extern ASSETS g_Assets;
