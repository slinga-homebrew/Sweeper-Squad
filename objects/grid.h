#pragma once
#include "object.h"

// individual cell
typedef struct _SQUARE
{
    //GAME_OBJECT;

    bool is_open; // true if already open
    bool is_bomb; // true if cell is a bomb

    int value; // count of neighboring bombs

} SQUARE, *PSQUARE;

// game grid
typedef struct _GRID
{
    GAME_OBJECT;

    int num_cols;
    int num_rows;
    int num_mines;

    SQUARE squares[40][18];

} GRID, *PGRID;

void initGrid(void);
void drawGrid(void);
void updateGrid(void);
