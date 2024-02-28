#include <jo/jo.h>
#include "../main.h"
#include "../assets.h"
#include "grid.h"
#include <string.h>

GRID g_Grid = {0};

// large array to randomize the mine locations
// avoid putting this on the stack
static unsigned int g_RandomMines[NUM_GRID_COLS * NUM_GRID_ROWS] = {0};

static void randomizeMines(void);

static void randomizeMines(void)
{
    memset(g_RandomMines, 0, sizeof(g_RandomMines));

    for(unsigned int i = 0; i < g_Grid.num_mines; i++)
    {
        g_RandomMines[i] = 1;
    }

    // first n positions are mines, now randomize the array
    shuffleArray(&g_RandomMines, COUNTOF(g_RandomMines));

    // set a mine in the places corresponding to the randomized array
    for(unsigned int i = 0; i < COUNTOF(g_RandomMines); i++)
    {
        if(g_RandomMines[i] == 1)
        {
            unsigned int column = i / NUM_GRID_ROWS;
            unsigned int row = i % NUM_GRID_ROWS;

            g_Grid.squares[column][row].is_bomb = true;

        }

    }
}

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

    //jo_printf(0, 27, "1 x: %d y: %d x2: %d y2: %d      ", x, y, g_Grid.num_cols, g_Grid.num_rows);
    //jo_printf(0, 28, "1 x: %d y: %d       ", x2, y2);

    //g_Grid.curPos.x = -312;
    //g_Grid.curPos.y = -144;

    // TODO: bug collision detection above grid
    if(y < -137)
    {
        return NULL;
    }

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


    //jo_printf(0, 28, "1 x: %d y: %d       ", x2, y2);

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
        return g_Assets.mine_exploded;
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

// initializes the grid based on game difficulty
// the higher the difficulty the more mines
void initGrid(GAME_DIFFICULTY gameDifficulty)
{
    memset(&g_Grid, 0, sizeof(g_Grid));

    switch(gameDifficulty)
    {
        // 420 total grids
        case GAME_DIFFICULTY_EASY:
            g_Grid.num_mines = 42; // 10% mines
            break;
        case GAME_DIFFICULTY_MEDIUM:
            g_Grid.num_mines = 84; // 20% mines
            break;
        case GAME_DIFFICULTY_HARD:
            g_Grid.num_mines = 126; // 30% mines
            break;
        default:
            // impossible to get here
            g_Grid.num_mines = 1;
            break;
    }

    g_Grid.num_cols = NUM_GRID_COLS;
    g_Grid.num_rows = NUM_GRID_ROWS;

    g_Grid.square_width = 21;
    g_Grid.square_height = 22;

    g_Grid.curPos.x = -315 + g_Grid.square_width/2;
    g_Grid.curPos.y = -143 + g_Grid.square_height/2;

    randomizeMines();
    calculateGridValues();

    g_Grid.objectState = OBJECT_STATE_ACTIVE;
}

// count the remaining non-mine, non-open squares
int countSquaresRemaining(void)
{
    int count = 0;

    for(int i = 0; i < g_Grid.num_cols; i++)
    {
        for(int j = 0; j < g_Grid.num_rows; j++)
        {
            PSQUARE square = &g_Grid.squares[i][j];

            if(square->is_open || square->is_bomb)
            {
                continue;
            }

            count++;
        }
    }

    return count;
}

// count the remaining mines
int countMinesRemaining(void)
{
    int count = g_Grid.num_mines;

    for(int i = 0; i < g_Grid.num_cols; i++)
    {
        for(int j = 0; j < g_Grid.num_rows; j++)
        {
            PSQUARE square = &g_Grid.squares[i][j];

            if(square->is_open && square->is_bomb)
            {
                count--;
            }
            else if(square->is_flagged)
            {
                count--;
            }
        }
    }

    // can happen if players flag too many squares as mines
    if(count < 0)
    {
        count = 0;
    }

    return count;
}

void drawGrid(void)
{
    if(g_Grid.objectState != OBJECT_STATE_ACTIVE)
    {
        return;
    }

    int width = g_Grid.square_width;
    int height = g_Grid.square_height;

    for(int i = 0; i < g_Grid.num_cols; i++)
    {
        for(int j = 0; j < g_Grid.num_rows; j++)
        {
            PSQUARE square = &g_Grid.squares[i][j];

            int sprite = getSquareSprite(square);
            jo_sprite_draw3D(sprite, g_Grid.curPos.x + (i*width) + 1, g_Grid.curPos.y + (j*height), GRID_Z);

            if(square->is_open == false && square->is_flagged)
            {
                jo_sprite_draw3D(g_Assets.flags[square->flag], g_Grid.curPos.x + (i*width) + 1, g_Grid.curPos.y + (j*height), GRID_Z);
            }
        }
    }

    return;
}

void updateGrid(void)
{
    return;
}

