#pragma once
#include "object.h"

#define NUM_GRID_COLS (30)
#define NUM_GRID_ROWS (14)

// individual cell
typedef struct _SQUARE
{
    //GAME_OBJECT;

    bool is_open; // true if already open
    bool is_bomb; // true if cell is a bomb
    bool is_flagged; // true if there is a flag on the cell

    int flag; // team id of the flag
    int value; // count of neighboring bombs

} SQUARE, *PSQUARE;

// game grid
typedef struct _GRID
{
    GAME_OBJECT;

    int num_cols;
    int num_rows;
    int num_mines;

    int square_width;
    int square_height;

    SQUARE squares[NUM_GRID_COLS][NUM_GRID_ROWS];

} GRID, *PGRID;

void initGrid(GAME_DIFFICULTY gameDifficulty);
void drawGrid(void);
void updateGrid(void);

PSQUARE playerToSquare(int x, int y, int* x3, int* y3);
void recursiveOpenSquares(int x, int y);

int countSquaresRemaining(void);
int countMinesRemaining(void);

