#pragma once

typedef enum _OBJECT_STATE
{
    OBJECT_STATE_INACTIVE = 0,
    OBJECT_STATE_ACTIVE,
} OBJECT_STATE;

typedef struct _FIXED_POSITION
{
    jo_fixed x;
    jo_fixed y;
    jo_fixed dx;
    jo_fixed dy;

} FIXED_POSITION, *PFIXED_POSITION;

typedef struct _GAME_OBJECT
{
    OBJECT_STATE objectState;
    FIXED_POSITION curPos;
} GAME_OBJECT, *PGAME_OBJECT;
