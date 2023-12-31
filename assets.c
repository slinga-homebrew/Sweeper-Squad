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
    #define NUM_MINE_SPRITES 5
    #define NUM_TEAM_SELECT_SPRITES 2
    #define NUM_PAUSE_LETTERS_SPRITES 6
    #define NUM_SCORE_DIGITS_SPRITES 10

    jo_tile cursors_tileset[NUM_CURSORS_SPRITES] = {0};
    jo_tile flags_tileset[NUM_FLAGS_SPRITES] = {0};
    jo_tile grid_tileset[NUM_GRID_SPRITES] = {0};
    jo_tile mines_tileset[NUM_MINE_SPRITES] = {0};
    jo_tile team_select_tileset[NUM_TEAM_SELECT_SPRITES] = {0};
    jo_tile pause_letters_tileset[NUM_PAUSE_LETTERS_SPRITES] = {0};
    jo_tile score_digits_tileset[NUM_SCORE_DIGITS_SPRITES] = {0};

    int palette_transparent_index = 2;

    // Set the global palette shared by all sprites in the game
    jo_set_tga_palette_handling(setGamePalette);
    g_Assets.title = jo_sprite_add_tga(NULL, "TITLE.TGA", palette_transparent_index);
    jo_set_tga_palette_handling(JO_NULL);

    //g_Assets.closed = jo_sprite_add_tga(NULL, "CLOSED.TGA", palette_transparent_index);
    //g_Assets.open = jo_sprite_add_tga(NULL, "OPEN.TGA", palette_transparent_index);

    initTileset(cursors_tileset, COUNTOF(cursors_tileset), 12, 16, 16);
    initTileset(flags_tileset, COUNTOF(flags_tileset), 12, 24, 22);
    initTileset(grid_tileset, COUNTOF(grid_tileset), 10, 24, 22);
    initTileset(mines_tileset, COUNTOF(mines_tileset), 5, 24, 22);
    initTileset(team_select_tileset, COUNTOF(team_select_tileset), 2, 48, 28);
    initTileset(pause_letters_tileset, COUNTOF(pause_letters_tileset), 6, 8, 8);
    initTileset(score_digits_tileset, COUNTOF(score_digits_tileset), 10, 16, 16);

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

    g_Assets.flags2[0] = jo_sprite_add_tga_tileset(NULL, "FLAGS2.TGA", palette_transparent_index, flags_tileset, COUNTOF(flags_tileset));
    for(unsigned int i = 0; i < 12; i++)
    {
        g_Assets.flags2[i] = g_Assets.flags2[0] + i;
    }

    //TODO: document
    g_Assets.closed = jo_sprite_add_tga_tileset(NULL, "GRID.TGA", palette_transparent_index, grid_tileset, COUNTOF(grid_tileset));
    for(unsigned int i = 0; i < 9; i++)
    {
        g_Assets.digits[i] = g_Assets.closed + i + 1;
    }

    g_Assets.mine = jo_sprite_add_tga_tileset(NULL, "MINES.TGA", palette_transparent_index, mines_tileset, COUNTOF(mines_tileset));
    g_Assets.mine_exploded = g_Assets.mine + 1;
    g_Assets.mine_first = g_Assets.mine + 2;
    g_Assets.mine_wrong = g_Assets.mine + 3;
    g_Assets.mine_wrong_ts = g_Assets.mine + 4;

    g_Assets.open_select = jo_sprite_add_tga_tileset(NULL, "TS_GRID.TGA", palette_transparent_index, team_select_tileset, COUNTOF(team_select_tileset));
    g_Assets.mine_exploded_select = g_Assets.open_select + 1;

    g_Assets.pause_c = jo_sprite_add_tga_tileset(NULL, "TABLEH.TGA", palette_transparent_index, pause_letters_tileset, COUNTOF(pause_letters_tileset));
    g_Assets.pause_d = g_Assets.pause_c + 1;
    g_Assets.pause_p = g_Assets.pause_c + 2;
    g_Assets.pause_r = g_Assets.pause_c + 3;
    g_Assets.pause_s = g_Assets.pause_c + 4;
    g_Assets.pause_t = g_Assets.pause_c + 5;

    g_Assets.horizontal_seperator = jo_sprite_add_tga(NULL, "SEPH.TGA", palette_transparent_index);
    g_Assets.vertical_seperator = jo_sprite_add_tga(NULL, "SEPV.TGA", palette_transparent_index);


    g_Assets.score_digits[0] = jo_sprite_add_tga_tileset(NULL, "S_DIGIT.TGA", palette_transparent_index, score_digits_tileset, COUNTOF(score_digits_tileset));
    for(unsigned int i = 0; i < 10; i++)
    {
        g_Assets.score_digits[i] = g_Assets.score_digits[0] + i;
    }
}
