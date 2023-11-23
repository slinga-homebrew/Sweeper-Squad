#include <jo/jo.h>
#include "../main.h"
#include "../assets.h"
#include "grid.h"

GRID g_Grid = {0};

int getSquareSprite(PSQUARE square)
{
    if(square->is_open == false)
    {
        return g_Assets.closed;
    }

    if(square->is_bomb == true)
    {
        return g_Assets.mine;
    }

    return g_Assets.digits[square->value];
}

int isSquareBomb(int x, int y)
{
    PSQUARE square = NULL;

    // make sure we are still in the grid
    if(x < 0 || x >= 40)
    {
        return 0;
    }

    if(y < 0 || y >= 18)
    {
        return 0;
    }

    square = &g_Grid.squares[x][y];

    if(square->is_bomb == false)
    {
        return 0;
    }

    return 1;
}

// calculate Square value based on how many neighboring cells are bombs
int calculateSquareValue(int x, int y)
{
    int value = 0;

    // above
    value += isSquareBomb(x - 1, y - 1);
    value += isSquareBomb(x, y - 1);
    value += isSquareBomb(x + 1, y - 1);

    // same row
    value += isSquareBomb(x - 1, y);
    value += isSquareBomb(x + 1, y);

    // below
    value += isSquareBomb(x - 1, y + 1);
    value += isSquareBomb(x, y + 1);
    value += isSquareBomb(x + 1, y + 1);

    return value;
}

void calculateGridValues(void)
{
    for(int i = 0; i < 40; i++)
    {
        for(int j = 0; j < 16; j++)
        {
            PSQUARE square = &g_Grid.squares[i][j];
            square->value = calculateSquareValue(i, j);
        }
    }
}

// TODO; needs to know grid size, num mines
void initGrid(void)
{
    memset(&g_Grid, 0, sizeof(g_Grid));

    g_Grid.curPos.x = -312;
    g_Grid.curPos.y = -144;

    for(int i = 0; i < 40; i++)
    {
        for(int j = 0; j < 16; j++)
        {
            PSQUARE square = &g_Grid.squares[i][j];

            square->is_open = true;//jo_random(2) - 1;
            square->is_bomb = !(jo_random(10) - 1);
            //square->value = jo_random(9) - 1;
        }
    }

    calculateGridValues();
    g_Grid.objectState = OBJECT_STATE_ACTIVE;
}

void drawGrid(void)
{
    if(g_Grid.objectState != OBJECT_STATE_ACTIVE)
    {
        return;
    }

    for(int i = 0; i < 40; i++)
    {
        for(int j = 0; j < 16; j++)
        {
            PSQUARE square = &g_Grid.squares[i][j];

            //square->value = 0;

            int sprite = getSquareSprite(square);
            jo_sprite_draw3D(sprite, g_Grid.curPos.x + (i*16) + 1, g_Grid.curPos.y + (j*22), 500);
        }
    }

    return;
}

void updateGrid(void)
{
    return;
}

