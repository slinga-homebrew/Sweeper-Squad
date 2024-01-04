#include <jo/jo.h>
#include <string.h>
#include "../main.h"
#include "../assets.h"
#include "player.h"
#include "grid.h"



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

void openSquare(PPLAYER player)
{
    PSQUARE square;
    int x2, y2;

    square = playerToSquare(toINT(player->curPos.x), toINT(player->curPos.y), &x2, &y2);
    if(!square)
    {
        // not on a square
        return;
    }

    if(square->is_open)
    {
        // already open
        return;
    }

    // check for bomb
    // increment score
    square->is_open = true;

    if(square->is_bomb)
    {
        return;
    }

    if(square->value != 0)
    {
        return;
    }

    square->is_open = false;

    recursiveOpenSquares(x2, y2);

}

void plantFlag(PPLAYER player)
{
    PSQUARE square;
    int x2, y2;

    square = playerToSquare(toINT(player->curPos.x), toINT(player->curPos.y), &x2, &y2);
    if(!square)
    {
        // not on a square
        return;
    }

    if(square->is_open)
    {
        // already open
        return;
    }

    square->is_flagged = !square->is_flagged;
    square->flag = player->playerID; // should be team id


    return;
}


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
            player->objectState = OBJECT_STATE_ACTIVE;
        }

        //player->animation = PLAYER_ANIMATION_IDLE;
        //player->hasSplashed = false;

        player->curPos.x = toFIXED((i*49.23) - 320 + 49.23);
        player->curPos.x = toFIXED(jo_random(640) - 320);

        // the player is initially not on a team
        player->teamSelectChoice = 6;

        //player->numBalloons = 2;

        //player->invulnerableTimer = 0; // make players invulnerable on spawn


        player->score.deaths = jo_random(2);
        player->score.points = jo_random(2);
        player->score.team_points = jo_random(2);
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

        if(player->subState == PLAYER_STATE_EXPLODING)
        {
            // can't move if exploding
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

        // did the player click
        if (jo_is_input_key_down(player->playerID, JO_KEY_A) ||
            jo_is_input_key_down(player->playerID, JO_KEY_C))
        {
            if(player->input.pressedAC == false)
            {
                openSquare(player);
            }
            player->input.pressedAC = true;

        }
        else
        {
            player->input.pressedAC = false;
        }

        // did the player plant a flag
        if (jo_is_input_key_down(player->playerID, JO_KEY_B))
        {
            if(player->input.pressedB == false)
            {
                plantFlag(player);
            }
            player->input.pressedB = true;

        }
        else
        {
            player->input.pressedB = false;
        }
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

        if(player->subState == PLAYER_STATE_EXPLODING)
        {
            player->frameCount--;
            if(player->frameCount <= 0)
            {
                player->objectState = OBJECT_STATE_INACTIVE;
            }
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

    //if(player->playerID >= 0 && player->playerID < 12)
    {
       playerSprite = g_Assets.cursors[player->playerID];
       jo_sprite_draw3D(playerSprite, toINT(player->curPos.x), toINT(player->curPos.y), PLAYER_Z);
    }

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

void explodePlayer(PPLAYER player)
{
    player->subState = PLAYER_STATE_EXPLODING;
    player->frameCount = EXPLODE_FRAME_COUNT;
    player->objectState = OBJECT_STATE_INACTIVE;
}
