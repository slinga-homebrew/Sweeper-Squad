#include <jo/jo.h>
#include "../main.h"
#include "../assets.h"
#include "player.h"
#include "explosion.h"
#include <string.h>

static int getExplosionSprite(PEXPLOSION explosion);

#define EXPLOSION_FRAME_COUNT (30)

EXPLOSION g_Explosions[MAX_TEAMS] = {0};

void initExplosions(void)
{
    memset(g_Explosions, 0, sizeof(g_Explosions));
}

void updateExplosions(void)
{
    for(unsigned int i = 0; i < COUNTOF(g_Explosions); i++)
    {
        PEXPLOSION explosion = &g_Explosions[i];

        if(explosion->objectState == OBJECT_STATE_INACTIVE)
        {
            continue;
        }

        // active explosion, update it's frame COUNTOF
        explosion->frameCount++;

        if(explosion->frameCount >= EXPLOSION_FRAME_COUNT)
        {
            explosion->objectState = OBJECT_STATE_INACTIVE;
        }
    }
}

void drawExplosions(void)
{
    int sprite = 0;

   for(unsigned int i = 0; i < COUNTOF(g_Explosions); i++)
    {
        PEXPLOSION explosion = &g_Explosions[i];

        if(explosion->objectState == OBJECT_STATE_INACTIVE)
        {
            continue;
        }

        sprite = getExplosionSprite(explosion);

        jo_sprite_draw3D(sprite, toINT(explosion->curPos.x), toINT(explosion->curPos.y), EXPLOSION_Z);
    }
}

// creates an explosion at the player's current position
void spawnExplosion(PPLAYER player)
{
    for(unsigned int i = 0; i < COUNTOF(g_Explosions); i++)
    {
        PEXPLOSION explosion = &g_Explosions[i];

        if(explosion->objectState == OBJECT_STATE_ACTIVE)
        {
            continue;
        }

        // explosion is centered on player
        explosion->curPos.x = player->curPos.x;
        explosion->curPos.y = player->curPos.y;

        explosion->frameCount = 0;

        jo_audio_play_sound(&g_Assets.explodePCM);
        explosion->objectState = OBJECT_STATE_ACTIVE;


        // we successfully created an explosion
        return;
    }

    // if we got here that means we were already maxed at explosions
    // no big deal, just return
    return;
}

static int getExplosionSprite(PEXPLOSION explosion)
{
    int frameNum = 0;

    frameNum = explosion->frameCount / 5;

    return g_Assets.explosions[frameNum];
}
