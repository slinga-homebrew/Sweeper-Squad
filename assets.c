#include <jo/jo.h>
#include "assets.h"

jo_palette g_GamePalette = {0};

static void loadSpriteAssets(void);
static jo_palette* setGamePalette(void);

// callback when an image is loaded
// Used to set the single palette shared by all of the game sprites
static jo_palette* setGamePalette(void)
{
    jo_create_palette(&g_GamePalette);

    // Not quite sure why I need to return the palette
    return &g_GamePalette;
}

// loads audio and graphic assets
void loadAssets(void)
{
    // performance optimization by ReyeMe: Dont' call
    jo_fs_cd("TEX");

    //loadFontAssets();
    loadSpriteAssets();
    //loadPCMAssets();

    /*
    for(unsigned int i = 0; i < COUNTOF(g_Game.randomizedPlayers); i++)
    {
        g_Game.randomizedPlayers[i] = i;
    }

    for(unsigned int i = 0; i < COUNTOF(g_Game.randomizedBalloons); i++)
    {
        g_Game.randomizedBalloons[i] = i;
    }

    for(unsigned int i = 0; i < COUNTOF(g_Game.randomizedFish); i++)
    {
        g_Game.randomizedFish[i] = i;
    }

    shuffleArray((unsigned int*)&g_Game.randomizedPlayers, COUNTOF(g_Game.randomizedPlayers));
    shuffleArray((unsigned int*)&g_Game.randomizedBalloons, COUNTOF(g_Game.randomizedBalloons));
    shuffleArray((unsigned int*)&g_Game.randomizedFish, COUNTOF(g_Game.randomizedFish));
    */

    // back to root dir
    jo_fs_cd("..");
}

// loads each sprite
// we are already CDed into the "TEX" directory
// each sprite is a 8 bpp paletted TGA
// each sprite uses the same palette
static void loadSpriteAssets(void)
{
    int palette_transparent_index = 2;

    // Set the global palette shared by all sprites in the game
    jo_set_tga_palette_handling(setGamePalette);
    g_Assets.title = jo_sprite_add_tga(NULL, "TITLE.TGA", palette_transparent_index);
    jo_set_tga_palette_handling(JO_NULL);


    g_Assets.closed = jo_sprite_add_tga(NULL, "CLOSED.TGA", palette_transparent_index);
    g_Assets.open = jo_sprite_add_tga(NULL, "OPEN.TGA", palette_transparent_index);

    g_Assets.cursors = jo_sprite_add_tga(NULL, "CURSORS.TGA", palette_transparent_index);

    g_Assets.flags = jo_sprite_add_tga(NULL, "FLAGS.TGA", palette_transparent_index);




}
