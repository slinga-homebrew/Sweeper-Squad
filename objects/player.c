#include <jo/jo.h>
#include <string.h>
#include "../main.h"
#include "../assets.h"
#include "player.h"
#include "grid.h"
#include "explosion.h"

#define X_SPEED_INC toFIXED(0.5)
#define Y_SPEED_INC toFIXED(4.0)
#define X_FRICTION_SPEED toFIXED(0.375)

#define MAX_Y_SPEED               toFIXED(8.0)  // don't fall faster than this
#define MAX_Y_SPEED2              toFIXED(-8.0)
#define MAX_X_SPEED               toFIXED(7.0)  // don't move faster than this
#define MAX_X_SPEED2              toFIXED(-7.0) // don't move faster than this

#define MAX_X_SPEED3               toFIXED(4.5)  // don't move faster than this
#define MAX_Y_SPEED3              toFIXED(-5.0)

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


#define EXPLODING_FRAME_COUNT (2 * 30)
#define EXPLODE_FRAME_COUNT (4 * 30)

// distance formula without the square root
bool checkDistance(PPLAYER a, PPLAYER b);


PLAYER g_Players[MAX_PLAYERS] = {0};

void drawPlayerSprite(PPLAYER player);
void speedLimitPlayer(PPLAYER player);
void boundPlayer(PPLAYER player);
void explodeNeighbors(PPLAYER player);

void respawnPlayer(PPLAYER player, bool deductLife);

static int getLives(GAME_MODE mode);

void spawnPlayers(void)
{
    PPLAYER player = NULL;

    for(unsigned int i = 0; i < COUNTOF(g_Players); i++)
    {
        player = &g_Players[i];
        respawnPlayer(player, false);
    }
}

void resetPlayerScores(void)
{
    PPLAYER player = NULL;

    for(unsigned int i = 0; i < COUNTOF(g_Players); i++)
    {
        player = &g_Players[i];

        player->score.deaths = 0;
        player->score.points = 0;
        player->score.team_points = 0;

        if(player->isPlaying)
        {
            player->numLives = getLives(g_Game.gameMode);
        }
        else
        {
            player->numLives = 0;
        }
    }
}

static void randomizeTitlePlayerPosition(int* x, int* y);

static void randomizeTitlePlayerPosition(int* x, int* y)
{
    *x = toFIXED(-1408 + jo_random(1408));
    *y = toFIXED(480 + jo_random(480));
}


static int getLives(GAME_MODE mode)
{
    int numLives = 0;

    switch(mode)
    {
        case GAME_MODE_PRACTICE:
            numLives = 1000; // basically infinite lives
            break;
        case GAME_MODE_NORMAL:
            numLives = 3;
            break;
        case GAME_MODE_HARDCORE:
            numLives = 1;
            break;
        case GAME_MODE_TIME_ATTACK:
            numLives = 3;
            break;
        default:
            numLives = 0; // should never get here
            break;
    }

    return numLives;

}

// initialize the title cursors that scroll in the background
void initTitlePlayers(void)
{
    PPLAYER player = NULL;

    memset(g_Players, 0, sizeof(g_Players));

    for(int i = 0; i < MAX_PLAYERS; i++)
    {
        player = &g_Players[i];

        player->objectState = OBJECT_STATE_ACTIVE;
        player->subState = PLAYER_STATE_TITLE;
        player->playerID = i;

        // cursor players start in the bottom left and move to the top right
        randomizeTitlePlayerPosition(&player->curPos.x, &player->curPos.y);

        player->curPos.dx = MAX_X_SPEED3;
        player->curPos.dy = MAX_Y_SPEED3;
    }

    return;
}

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
        explodePlayer(player, true, true);
        return;
    }

    player->score.points++;

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
    square->flag = player->teamID;


    return;
}


void initPlayers(void)
{
    PPLAYER player = NULL;
    int numLives = 0;

    memset(g_Players, 0, sizeof(g_Players));

    numLives = getLives(g_Game.gameMode);

    for(unsigned int i = 0; i < COUNTOF(g_Players); i++)
    {
        player = &g_Players[i];

        player->playerID = i;

        // TODO: one player for now
        player->objectState = OBJECT_STATE_ACTIVE;

        // the player is initially not on a team
        player->teamSelectChoice = 6;

        player->numLives = numLives;
        player->crackChoice = jo_random(4) - 1;
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

        if(player->subState == PLAYER_STATE_EXPLODING ||
           player->subState == PLAYER_STATE_EXPLODED ||
           player->subState == PLAYER_STATE_TITLE ||
           player->subState == PLAYER_STATE_DEAD)
        {
            // uncontrollable in these states
            continue;
        }

        if(player->subState == PLAYER_STATE_LOGO)
        {
            // moving towards the center
            if(toINT(player->curPos.x) < 0)
            {
                player->curPos.dx += X_SPEED_INC;
            }
            else if(toINT(player->curPos.x) > 0)
            {
                player->curPos.dx -= X_SPEED_INC;
            }

            // moving towards the center
            if(toINT(player->curPos.y) < 0)
            {
                player->curPos.dy += X_SPEED_INC;
            }
            else if(toINT(player->curPos.y) > 0)
            {
                player->curPos.dy -= Y_SPEED_INC;
            }

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
                // transition from exploding to exploded state
                //player->objectState = OBJECT_STATE_INACTIVE;
                player->curPos.dx = 0;
                player->curPos.dy = 0;
                player->ds = 0;
                player->da = 0;

                player->frameCount = EXPLODE_FRAME_COUNT;
                player->subState = PLAYER_STATE_EXPLODED;
                jo_audio_play_sound(&g_Assets.crackPCM);

            }
            //continue;
        }

        if(player->subState == PLAYER_STATE_EXPLODED)
        {
            player->frameCount--;
            if(player->frameCount <= 0)
            {
                respawnPlayer(player, true);
            }
        }

        speedLimitPlayer(player);

        // move the player
        player->curPos.y += player->curPos.dy;
        player->curPos.x += player->curPos.dx;
        player->size += player->ds;
        player->angle += player->da;

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

        drawPlayerSprite(player);
    }
}

void drawPlayerSprite(PPLAYER player)
{
    int playerSprite = 0;
    int crackSprite = 0;

    playerSprite = g_Assets.cursors[g_Assets.randomizedColors[player->playerID]];

    // don't draw dead players
    if(player->subState == PLAYER_STATE_DEAD)
    {
        return;
    }

    if(player->subState == PLAYER_STATE_EXPLODING || player->subState == PLAYER_STATE_EXPLODED)
    {


        if(player->subState == PLAYER_STATE_EXPLODED)
        {
            crackSprite = g_Assets.cracks[player->crackChoice];
            jo_sprite_change_sprite_scale_xy_fixed(toFIXED(3), toFIXED(3));
            jo_sprite_draw3D(crackSprite, toINT(player->curPos.x), toINT(player->curPos.y), CRACKS_Z);
            jo_sprite_restore_sprite_scale();
        }

        jo_sprite_change_sprite_scale_xy_fixed(player->size, player->size);


        jo_sprite_draw3D_and_rotate(playerSprite, toINT(player->curPos.x), toINT(player->curPos.y), PLAYER_Z, player->angle);

        //jo_sprite_draw3D(playerSprite, toINT(player->curPos.x), toINT(player->curPos.y), PLAYER_Z);

        jo_sprite_restore_sprite_scale();


        return;
    }

    jo_sprite_draw3D(playerSprite, toINT(player->curPos.x), toINT(player->curPos.y), PLAYER_Z);


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

    if(player->curPos.dy < MAX_Y_SPEED2 - bonusSpeed)
    {
        player->curPos.dy = MAX_Y_SPEED2 - bonusSpeed;
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


    // ignore wrap on logo state?
    if(player->subState == PLAYER_STATE_LOGO)
    {
        return;
    }

    if(player->subState == PLAYER_STATE_TITLE)
    {
        if(player->curPos.x > SCREEN_RIGHT || player->curPos.y < SCREEN_TOP)
        {
            //player->curPos.x = SCREEN_LEFT;
            randomizeTitlePlayerPosition(&player->curPos.x, &player->curPos.y);
        }



        return;
    }

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

void explodePlayer(PPLAYER player, bool showExplosion, bool spreadExplosion)
{
    int rand = 0;

    player->subState = PLAYER_STATE_EXPLODING;
    player->frameCount = EXPLODING_FRAME_COUNT + jo_random(EXPLODING_FRAME_COUNT);
    player->score.deaths++;

    player->curPos.dx = jo_random(0x40000) - 0x20000;
    player->curPos.dy = jo_random(0x40000) - 0x20000;

    player->size = toFIXED(1);
    player->ds = toFIXED(.07);

    player->angle = jo_random(360);


    player->da = 45 - jo_random(30);

    // 50% chance to flip the direction
    rand = jo_random(2);
    if(rand == 1)
    {
        player->da *= -1;
    }

    if(showExplosion)
    {
        // draw an explosion on the player
        spawnExplosion(player);
    }

    if(spreadExplosion)
    {
        // spread the explosion to nearby players
        explodeNeighbors(player);
    }
}

void explodeNeighbors(PPLAYER player)
{
    bool neighbor = false;

    for(unsigned int i = 0; i < COUNTOF(g_Players); i++)
    {
        PPLAYER victim = &g_Players[i];

        if(victim->objectState != OBJECT_STATE_ACTIVE)
        {
            continue;
        }

        if(victim->subState != PLAYER_STATE_ACTIVE)
        {
            continue;
        }

        if(player->playerID == victim->playerID)
        {
            continue;
        }

        // check if player is nearby
        neighbor = checkDistance(player, victim);
        if(neighbor == true)
        {
            explodePlayer(victim, false, false);
        }
    }
}

void respawnPlayer(PPLAYER player, bool deductLife)
{
    if(player->objectState != OBJECT_STATE_ACTIVE)
    {
        return;
    }

    if(g_Game.gameState != GAME_STATE_GAMEPLAY)
    {
        // only respawn during gameplay
        player->subState = PLAYER_STATE_DEAD;
        return;
    }

    if(player->numLives <= 0)
    {
        // no lives
        player->subState = PLAYER_STATE_DEAD;
        return;
    }

    if(deductLife == true)
    {
        player->numLives--;
    }

    // set position
    player->curPos.x = toFIXED(jo_random(660) - 330);
    player->curPos.y = toFIXED(jo_random(330) - 150);
    player->angle = 0;
    player->size = toFIXED(1);

    // reset speed to zero
    player->curPos.dx = 0;
    player->curPos.dy = 0;
    player->da = 0;
    player->ds = 0;

    player->subState = PLAYER_STATE_ACTIVE;
}

// distance formula without the square root
bool checkDistance(PPLAYER a, PPLAYER b)
{
    int dist = 36*36;

    int x_dist = toINT(a->curPos.x) - toINT(b->curPos.x);
    x_dist = x_dist * x_dist;

    int y_dist = toINT(a->curPos.y) - toINT(b->curPos.y);
    y_dist = y_dist * y_dist;

    if(dist > x_dist + y_dist)
    {
        return true;
    }

    return false;

}
