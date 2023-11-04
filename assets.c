#include <jo/jo.h>
#include "assets.h"

void loadSpriteAssets()
{
    jo_fs_cd("TEX");
    g_Assets.title = jo_sprite_add_tga(NULL, "TITLE.TGA", JO_COLOR_Transparent);

    // back to root dir
    jo_fs_cd("..");
}
