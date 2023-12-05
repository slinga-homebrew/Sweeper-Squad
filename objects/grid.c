#include <jo/jo.h>
#include "../main.h"
#include "../assets.h"
#include "grid.h"

GRID g_Grid = {0};

void recursiveOpenSquares(int x, int y)
{
    PSQUARE square = NULL;

    if(x < 0 || x >= g_Grid.num_cols)
    {
        return;
    }

    if(y < 0 || y >= g_Grid.num_rows)
    {
        return;
    }

    square = &g_Grid.squares[x][y];
    if(square->is_open)
    {
        return;
    }

    if(square->is_bomb)
    {
        return;
    }

    square->is_open = true;


    if(square->value != 0)
    {
        return;
    }

    recursiveOpenSquares(x-1, y-1);
    recursiveOpenSquares(x, y-1);
    recursiveOpenSquares(x+1, y-1);

    recursiveOpenSquares(x-1, y);
    recursiveOpenSquares(x+1, y);

    recursiveOpenSquares(x-1, y+1);
    recursiveOpenSquares(x, y+1);
    recursiveOpenSquares(x+1, y+1);

    return;
}

PSQUARE playerToSquare(int x, int y, int* x3, int* y3)
{
    int x2 = 0;
    int y2 = 0;

    jo_printf(0, 27, "1 x: %d y: %d x2: %d y2: %d      ", x, y, g_Grid.num_cols, g_Grid.num_rows);
    jo_printf(0, 28, "1 x: %d y: %d       ", x2, y2);

    //g_Grid.curPos.x = -312;
    //g_Grid.curPos.y = -144;

    // TODO: make this variables
    x2 = (x - g_Grid.curPos.x) / g_Grid.square_width;
    y2 = (y - g_Grid.curPos.y) / g_Grid.square_height;

    if(x2 < 0 || x2 >= g_Grid.num_cols)
    {
        return NULL;
    }

    if(y2 < 0 || y2 >= g_Grid.num_rows)
    {
        return NULL;
    }


    jo_printf(0, 28, "1 x: %d y: %d       ", x2, y2);

    *x3 = x2;
    *y3 = y2;

    return &g_Grid.squares[x2][y2];
}

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
    if(x < 0 || x >= g_Grid.num_cols)
    {
        return 0;
    }

    if(y < 0 || y >= g_Grid.num_rows)
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
    for(int i = 0; i < g_Grid.num_cols; i++)
    {
        for(int j = 0; j < g_Grid.num_rows; j++)
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

    g_Grid.num_cols = 30;
    g_Grid.num_rows = 15;

    g_Grid.square_width = 21;
    g_Grid.square_height = 22;

    g_Grid.curPos.x = -315 + g_Grid.square_width/2;
    g_Grid.curPos.y = -165 + g_Grid.square_height/2;


    for(int i = 0; i < g_Grid.num_cols; i++)
    {
        for(int j = 0; j < g_Grid.num_rows; j++)
        {
            PSQUARE square = &g_Grid.squares[i][j];

            square->is_open = false;//jo_random(2) - 1;
            square->is_bomb = !(jo_random(5) - 1);
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

    //jo_sprite_change_sprite_scale(2);

    int width = g_Grid.square_width;
    int height = g_Grid.square_height;

    for(int i = 0; i < g_Grid.num_cols; i++)
    {
        for(int j = 0; j < g_Grid.num_rows; j++)
        {
            PSQUARE square = &g_Grid.squares[i][j];

            //square->value = 0;

            int sprite = getSquareSprite(square);
            jo_sprite_draw3D(sprite, g_Grid.curPos.x + (i*width) + 1, g_Grid.curPos.y + (j*height), 500);

            if(square->is_open == false && square->is_flagged)
            {
                jo_sprite_draw3D(g_Assets.flags[0], g_Grid.curPos.x + (i*width) + 1, g_Grid.curPos.y + (j*height), 500);
            }
        }
    }

    //jo_sprite_change_sprite_scale(1);


    return;
}

void updateGrid(void)
{
    return;
}

