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

static void initTileset(jo_tile* tileset, unsigned int numSprites, unsigned int spritesPerRow, int width, int height)
{
    unsigned int col = 0;
    unsigned int row = 0;

    for(unsigned int i = 0; i < numSprites; i++)
    {
        col = i % spritesPerRow;
        row = i / spritesPerRow;

        tileset[i].x = col * width;
        tileset[i].y = row * height;
        tileset[i].height = height;
        tileset[i].width = width;
    }
}

// loads each sprite
// we are already CDed into the "TEX" directory
// each sprite is a 8 bpp paletted TGA
// each sprite uses the same palette
static void loadSpriteAssets(void)
{
    #define NUM_CURSORS_SPRITES 12
    #define NUM_FLAGS_SPRITES 12
    #define NUM_GRID_SPRITES 10

    jo_tile cursors_tileset[NUM_CURSORS_SPRITES] = {0};
    jo_tile flags_tileset[NUM_FLAGS_SPRITES] = {0};
    jo_tile grid_tileset[NUM_GRID_SPRITES] = {0};

    int palette_transparent_index = 2;

    // Set the global palette shared by all sprites in the game
    jo_set_tga_palette_handling(setGamePalette);
    g_Assets.title = jo_sprite_add_tga(NULL, "TITLE.TGA", palette_transparent_index);
    jo_set_tga_palette_handling(JO_NULL);


    //g_Assets.closed = jo_sprite_add_tga(NULL, "CLOSED.TGA", palette_transparent_index);
    g_Assets.open = jo_sprite_add_tga(NULL, "OPEN.TGA", palette_transparent_index);

    g_Assets.mine = jo_sprite_add_tga(NULL, "MINE.TGA", palette_transparent_index);

    initTileset(cursors_tileset, COUNTOF(cursors_tileset), 12, 16, 16);
    initTileset(flags_tileset, COUNTOF(flags_tileset), 12, 16, 22);
    initTileset(grid_tileset, COUNTOF(grid_tileset), 10, 16, 22);

    g_Assets.cursors[0] = jo_sprite_add_tga_tileset(NULL, "CURSORS.TGA", palette_transparent_index, cursors_tileset, COUNTOF(cursors_tileset));
    for(unsigned int i = 0; i < 12; i++)
    {
        g_Assets.cursors[i] = g_Assets.cursors[0] + i;
    }

    g_Assets.flags[0] = jo_sprite_add_tga_tileset(NULL, "FLAGS.TGA", palette_transparent_index, flags_tileset, COUNTOF(flags_tileset));
    for(unsigned int i = 0; i < 12; i++)
    {
        g_Assets.flags[i] = g_Assets.flags[0] + i;
    }

    //TODO: document
    g_Assets.closed = jo_sprite_add_tga_tileset(NULL, "GRID.TGA", palette_transparent_index, grid_tileset, COUNTOF(grid_tileset));
    for(unsigned int i = 0; i < 9; i++)
    {
        g_Assets.digits[i] = g_Assets.closed + i;
    }





}
