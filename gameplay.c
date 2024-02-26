#include <jo/jo.h>
#include "main.h"
#include "gameplay.h"
#include "objects/grid.h"
#include "objects/player.h"
#include "assets.h"
#include "pause.h"
#include "objects/explosion.h"
#include "audio.h"

extern PLAYER g_Players[MAX_PLAYERS];

typedef struct _TEAM_SCORE
{
    int team_id;
    int points;
    int time;
} TEAM_SCORE, *PTEAM_SCORE;


bool g_RoundOver = false;
int g_RoundOverTimer = 0;

#define ROUND_OVER_TIME_FRAMES (30*4)

static bool isRoundOver(void);
static void drawStats(void);
static void drawScore(void);

static void drawStats(void)
{
    int numSquares = 0;
    int numMines = 0;

    int yPos = -192;
    int xPos = 0-90;

    unsigned char digit1 = 0;
    unsigned char digit2 = 0;
    unsigned char digit3 = 0;
    int letterSpacing = 30;

    numSquares = countSquaresRemaining();
    convertNumberToDigits(numSquares, &digit1, &digit2, &digit3);

    jo_sprite_change_sprite_scale(2);
    jo_sprite_draw3D(g_Assets.scores[0][digit1], xPos + (letterSpacing*0), yPos, SCORE_Z);
    jo_sprite_draw3D(g_Assets.scores[0][digit2], xPos + (letterSpacing*1), yPos, SCORE_Z);
    jo_sprite_draw3D(g_Assets.scores[0][digit3], xPos + (letterSpacing*2), yPos, SCORE_Z);

    xPos = 30- 4;

    numMines = countMinesRemaining();
    convertNumberToDigits(numMines, &digit1, &digit2, &digit3);

    jo_sprite_draw3D(g_Assets.scores[1][digit1], xPos + (letterSpacing*0), yPos, SCORE_Z);
    jo_sprite_draw3D(g_Assets.scores[1][digit2], xPos + (letterSpacing*1), yPos, SCORE_Z);
    jo_sprite_draw3D(g_Assets.scores[1][digit3], xPos + (letterSpacing*2), yPos, SCORE_Z);

    jo_sprite_change_sprite_scale(1);
}

// sort players by score for coop mode
static int sortByTeamScore(const void * teamScore1, const void * teamScore2)
{
    PTEAM_SCORE a = (PTEAM_SCORE)teamScore1;
    PTEAM_SCORE b = (PTEAM_SCORE)teamScore2;

    // sort by score
    if(b->points > a->points)
    {
        return 1;
    }
    else if(b->points < a->points)
    {
        return -1;
    }

    // tie-breaker: lower team ID wins
    if(b->team_id < a->team_id)
    {
        return 1;
    }
    else
    {
        return -1;
    }

    return 0;
}


static void drawScore(void)
{
    int yPos = -200;
    int xPos = -308;

    unsigned char digit1 = 0;
    unsigned char digit2 = 0;
    unsigned char digit3 = 0;
    int letterSpacing = 15;

    TEAM_SCORE scores[MAX_TEAMS] = {0};

    for(unsigned int i = 0; i < COUNTOF(scores); i++)
    {
        scores[i].team_id = i;
    }

    for(unsigned int j = 0; j < COUNTOF(g_Players); j++)
    {
        PPLAYER player = &g_Players[j];

        if(player->objectState != OBJECT_STATE_ACTIVE)
        {
            continue;
        }

        scores[player->teamSelectChoice].points += player->score.points;
    }


    qsort(scores, COUNTOF(scores), sizeof(TEAM_SCORE), sortByTeamScore);


    /*
    digit1 = (team_points / 100);
    digit2 = ((team_points - ((team_points/100) * 100))/10);
    digit3 = (team_points % 10);
    */

    for(int i = 0; i < 12; i++)
    {
        if(i < 6)
        {
            yPos = -200;
            xPos = -308 + (i * (32 + (letterSpacing*3)));
        }
        else
        {
            yPos = -200+24;
            xPos = -308 + ((i-6) * (32 + (letterSpacing*3)));
        }

        if(i >= 3 && i <= 5)
        {
            xPos += 196;
        }
        else if(i >= 9)
        {
            xPos += 196;
        }

        unsigned int color = scores[i].team_id;
        unsigned int score  =  scores[i].points;
        convertNumberToDigits(score, &digit1, &digit2, &digit3);

        jo_sprite_draw3D(g_Assets.scores[color][digit1], xPos + (letterSpacing*0), yPos, SCORE_Z);
        jo_sprite_draw3D(g_Assets.scores[color][digit2], xPos + (letterSpacing*1), yPos, SCORE_Z);
        jo_sprite_draw3D(g_Assets.scores[color][digit3], xPos + (letterSpacing*2), yPos, SCORE_Z);

    }
}

int countActiveTeams()
{
    int teamCount[12] = {0}; // number of players on each team
    int activeTeams = 0;

    for(unsigned int i = 0; i < COUNTOF(g_Players); i++)
    {
        PPLAYER player = &g_Players[i];

        if(player->objectState == OBJECT_STATE_INACTIVE)
        {
            continue;
        }

        if(player->subState == PLAYER_STATE_DEAD)
        {
            continue;
        }

        teamCount[player->teamSelectChoice] = teamCount[player->teamSelectChoice] + 1;
    }

    for(unsigned int j = 0; j < COUNTOF(teamCount); j++)
    {
        if(teamCount[j] > 0)
        {
            activeTeams++;
        }
    }

    return activeTeams;
}

static bool isRoundOver(void)
{
    int numTeams = 0;
    int numSquares = 0;

    // TODO: check for no time left in time attack
    numTeams = countActiveTeams();
    if(numTeams == 0)
    {
        // no more teams left, players lost
        g_RoundOver = true;
        g_RoundOverTimer = ROUND_OVER_TIME_FRAMES;
        playCDTrack(DEATH_TRACK);
        return true;
    }

    numSquares = countSquaresRemaining();
    if(numSquares == 0)
    {
        // no more squares left to open, players win
        g_RoundOver = true;
        g_RoundOverTimer = ROUND_OVER_TIME_FRAMES;
        playCDTrack(VICTORY_TRACK);
        return true;
    }

    return false;
}

void gameplay_init(void)
{

    // init grid?
    //getPlayerSpawnLocations(g_Players, false);

    initGrid(g_Game.gameDifficulty);
    resetPlayerScores();
    spawnPlayers();

    initExplosions();

    // todo:
    g_Game.isPaused = false;
    //g_Game.isPaused = true;


    g_RoundOver = false;

}

void gameplay_input(void)
{
    if(g_Game.gameState != GAME_STATE_GAMEPLAY)
    {
        return;
    }

    if(g_Game.isPaused == true)
    {
        return;
    }

    getPlayersInput();
}

void gameplay_update(void)
{
    if(g_Game.gameState != GAME_STATE_GAMEPLAY)
    {
        return;
    }

    // don't do anything if the game is paused
    if(g_Game.isPaused == true)
    {
        return;
    }

    updatePlayers();
    updateExplosions();

    if(g_RoundOver == true)
    {
        g_RoundOverTimer--;

        // check if it's time to go to the pause screen
        if(g_RoundOverTimer <= 0)
        {
            g_Game.isPaused = true;
        }
        return;
    }

    g_RoundOver = isRoundOver();
}

void gameplay_draw(void)
{
    if(g_Game.gameState != GAME_STATE_GAMEPLAY)
    {
        return;
    }

    if(g_Game.isPaused == true)
    {
        return;
    }

    drawGrid();
    drawPlayers();
    drawScore();
    drawStats();
    drawExplosions();
}
