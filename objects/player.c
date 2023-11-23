#include <jo/jo.h>
#include "../main.h"
#include "../assets.h"
#include "player.h"


#define X_SPEED_INC toFIXED(0.5)
#define Y_SPEED_INC toFIXED(4.0)
#define X_FRICTION_SPEED toFIXED(0.375)

#define MAX_Y_SPEED               toFIXED(7.0)  // don't fall faster than this
#define MAX_Y_SPEED2              toFIXED(7.0)
#define MAX_X_SPEED               toFIXED(6.0)  // don't move faster than this
#define MAX_X_SPEED2              toFIXED(-6.0) // don't move faster than this


// TODO: move this
// screen definition
#define SCREEN_RIGHT  toFIXED(352.0)
#define SCREEN_LEFT  toFIXED(-352.0)
#define SCREEN_WIDTH  toFIXED(704.0)

#define SCREEN_TOP toFIXED(-240.0)
#define SCREEN_BOTTOM toFIXED(240.0)

#define SCREEN_OFFSET toFIXED(100)
#define GAMEPLAY_ABOVE_SCREEN (SCREEN_TOP - SCREEN_OFFSET)
#define GAMEPLAY_BELOW_SCREEN (SCREEN_BOTTOM + SCREEN_OFFSET)

PLAYER g_Players[MAX_PLAYERS] = {0};

void drawPlayerSprite(PPLAYER player);
void speedLimitPlayer(PPLAYER player);
void boundPlayer(PPLAYER player);


void initPlayers(void)
{
    PPLAYER player = NULL;

    memset(g_Players, 0, sizeof(g_Players));

    for(unsigned int i = 0; i < COUNTOF(g_Players); i++)
    {
        player = &g_Players[i];

        //memset(&player->curPos, 0, sizeof(FIXED_POSITION));
        //memset(&player->oldPos, 0, sizeof(FIXED_POSITION));
        //memset(&player->startPos, 0, sizeof(FIXED_POSITION));

        player->playerID = i;

        // TODO: one player for now
        if(i == 0)
        {
            player->objectState = OBJECT_STATE_ACTIVE;
        }
        else
        {
            player->objectState = OBJECT_STATE_INACTIVE;
        }

        //player->animation = PLAYER_ANIMATION_IDLE;
        //player->hasSplashed = false;

        player->curPos.x = toFIXED((i*49.23) - 320 + 49.23);
        player->curPos.x = toFIXED(jo_random(640) - 320);

        // the player is initially not on a team
        player->teamSelectChoice = 6;

        //player->numBalloons = 2;

        //player->invulnerableTimer = 0; // make players invulnerable on spawn
    }
}


void getPlayersInput(void)
{
    PPLAYER player = NULL;

    // check inputs for all players
    for(unsigned int i = 0; i < COUNTOF(g_Players); i++)
    {
        player = &g_Players[i];

        //copyPos(&player->oldPos, &player->curPos);

        //getDebugPlayersInput();
        //return;

        if(player->objectState == OBJECT_STATE_INACTIVE)
        {
            // TODO: hit a button to respawn??
            continue;
        }

        /*
        if(player->animation == PLAYER_ANIMATION_ELECTROCUTING ||
           player->animation == PLAYER_ANIMATION_CRYING ||
           player->animation == PLAYER_ANIMATION_DROPPING ||
           player->animation == PLAYER_ANIMATION_EATING )
        {
            continue;
        }
        */

        player->moveHorizontal = false;

        if (jo_is_input_key_pressed(player->playerID, JO_KEY_LEFT))
        {
            if(player->input.pressedLeft == false)
            {
                player->curPos.dx -= X_SPEED_INC;
                //player->dirRight = false;
                player->moveHorizontal = true;
            }
            //player->input.pressedLeft = true;
        }
        else
        {
            player->input.pressedLeft = false;
        }

        if (jo_is_input_key_pressed(player->playerID, JO_KEY_RIGHT))
        {
            if(player->input.pressedRight == false)
            {
                player->curPos.dx += X_SPEED_INC;
                //player->dirRight = true;
                player->moveHorizontal = true;
            }
            //player->input.pressedRight = true;
        }
        else
        {
            player->input.pressedRight = false;

        }

        if(player->moveHorizontal == false)
        {
            player->curPos.dx = 0;
        }

        player->moveVertical = false;

        if (jo_is_input_key_pressed(player->playerID, JO_KEY_UP))
        {
            if(player->input.pressedUp == false)
            {
                player->curPos.dy -= X_SPEED_INC;
                //player->dirRight = false;
                player->moveVertical = true;
            }
            //player->input.pressedLeft = true;
        }
        else
        {
            player->input.pressedUp = false;
        }

        if (jo_is_input_key_pressed(player->playerID, JO_KEY_DOWN))
        {
            if(player->input.pressedDown == false)
            {
                player->curPos.dy += X_SPEED_INC;
                //player->dirRight = true;
                player->moveVertical = true;
            }
            //player->input.pressedRight = true;
        }
        else
        {
            player->input.pressedDown = false;

        }

        if(player->moveVertical == false)
        {
            player->curPos.dy = 0;
        }


        /*
        // did the player flap
        // if the player is dead, flapping respawns them
        if (jo_is_input_key_down(player->playerID, JO_KEY_A) ||
            jo_is_input_key_down(player->playerID, JO_KEY_B) ||
            jo_is_input_key_down(player->playerID, JO_KEY_C))
        {
            //if(player->input.pressedABC == false)
            {
                //player->input.pressedABC = true;
                player->hasFlapped = true;
                player->flapping = true;

                if(player->animation == PLAYER_ANIMATION_FLYING)
                {
                    player->frameTimer += 8;
                    player->frameTimer = player->frameTimer & 0xf;
                }
                else
                {
                    player->frameTimer = 8;
                }
            }
        }
        else
        {
            player->input.pressedAC = false;
            player->input.pressedB = false;
        }

        // calculate the player's y speed
        if(player->flapping == true)
        {
            //player->flapping = !player->flapping;
            //player->frameTimer = FLICKY_FLAPPING_SPEED;
            player->curPos.dy -= Y_SPEED_INC;
        }
        else
        {
            if(player->curPos.dy < MAX_Y_SPEED)
            {
                player->curPos.dy += GRAVITY_CONSTANT;
            }
        }
        */
    }
}

void updatePlayers(void)
{
    PPLAYER player = NULL;

    for(unsigned int i = 0; i < COUNTOF(g_Players); i++)
    {
        player = &g_Players[i];

        if(player->objectState == OBJECT_STATE_INACTIVE)
        {
            continue;
        }




        /*
        // don't update the player if dramatic death is happening
        if(g_Game.dramaticDeathCounter > 0)
        {
            if(player->animation != PLAYER_ANIMATION_EATING)
            {
                return;
            }
        }
        */

        speedLimitPlayer(player);

        player->curPos.y += player->curPos.dy;
        player->curPos.x += player->curPos.dx;

        boundPlayer(player);
    }
}

void drawPlayers(void)
{
    for(unsigned int i = 0; i <  COUNTOF(g_Players); i++)
    {
        PPLAYER player = &g_Players[i];

        if(player->objectState == OBJECT_STATE_INACTIVE)
        {
            continue;
        }

        if(i == 0)
        {
            jo_printf(0, 27, "x: %d y: %d       ", toINT(player->curPos.x), toINT(player->curPos.y));
        }

        /*
        // make invulnerable players flicker
        if(player->invulnerableTimer > 0)
        {
            if((player->invulnerableTimer % 5) == 0)
            {
                continue;
            }
        }
        */

        /*
        if(player->dirRight == true)
        {
            jo_sprite_enable_horizontal_flip();
        }
        */

        //jo_printf(13, 12+i, "Player %d %d          active", i, MAX_PLAYERS);

        drawPlayerSprite(player);

    }
}

void drawPlayerSprite(PPLAYER player)
{
    int playerSprite = 0;

    /*
    if(player->dirRight == true)
    {
        jo_sprite_enable_horizontal_flip();
    }

    playerSprite = getPlayerSprite(player);
    */

    //playerSprite = g_Assets.cursors[player->playerID];

    if(player->playerID >= 0 && player->playerID < 12)
    {
       playerSprite = g_Assets.cursors[player->playerID];
       jo_sprite_draw3D(playerSprite, toINT(player->curPos.x), toINT(player->curPos.y), PLAYER_Z);
    }

    //jo_sprite_change_sprite_scale(3);


    //jo_sprite_draw3D(g_Assets.fish[4][2], toINT(player->curPos.x), toINT(player->curPos.y), FISH_Z);
    //jo_sprite_change_sprite_scale(1);

    /*
    // draw another sprite if the player has wrapped the screen
    // todo: check if the player is out of position before drawing
    if(player->curPos.x  + toFIXED(8.0) > SCREEN_RIGHT)
    {
        jo_sprite_draw3D(playerSprite, toINT(player->curPos.x - SCREEN_WIDTH), toINT(player->curPos.y), PLAYER_Z);
    }

    // draw another sprite if the player has wrapped the screen
    if(player->curPos.x - toFIXED(8.0) < SCREEN_LEFT)
    {
        jo_sprite_draw3D(playerSprite, toINT(player->curPos.x + SCREEN_WIDTH), toINT(player->curPos.y), PLAYER_Z);
    }

    if(player->dirRight == true)
    {
        jo_sprite_disable_horizontal_flip();
    }
    */
}


void speedLimitPlayer(PPLAYER player)
{
    jo_fixed bonusSpeed = 0;

    /*
    if(player->exceedLimitTimer > 0)
    {
        player->exceedLimitTimer--;
        bonusSpeed = toFIXED(4.0);
    }
    */

    // validate speeds
    if(player->curPos.dx > MAX_X_SPEED + bonusSpeed)
    {
        player->curPos.dx = MAX_X_SPEED + bonusSpeed;
    }

    if(player->curPos.dx < MAX_X_SPEED2 - bonusSpeed)
    {
        player->curPos.dx = MAX_X_SPEED2 - bonusSpeed;
    }

    if(player->curPos.dy > MAX_Y_SPEED + bonusSpeed)
    {
        player->curPos.dy = MAX_Y_SPEED + bonusSpeed;
    }

    if(player->curPos.dy < -MAX_Y_SPEED2 - bonusSpeed)
    {
        player->curPos.dy = -MAX_Y_SPEED2 - bonusSpeed;
    }
}

void boundPlayer(PPLAYER player)
{
    /*
    if(player->animation == PLAYER_ANIMATION_DROPPING)
    {
        // a dying player creates a splash after they hit the water level
        if(player->hasSplashed == false && toINT(player->curPos.y) >= 200)
        {
            player->hasSplashed = true;
            initSplash(player->curPos.x);
        }
        return;
    }
    */

    // screen boundaries
    if(player->curPos.x > SCREEN_RIGHT)
    {
        player->curPos.x = SCREEN_LEFT;
    }

    if(player->curPos.x < SCREEN_LEFT)
    {
        player->curPos.x = SCREEN_RIGHT;
    }

    if(player->curPos.y > SCREEN_BOTTOM)
    {
        player->curPos.y = SCREEN_TOP;
    }

    if(player->curPos.y < SCREEN_TOP)
    {
        player->curPos.y = SCREEN_BOTTOM;
    }


    return;
}
