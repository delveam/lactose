#ifndef CONSTANTS_H
#define CONSTANTS_H

#include "raylib.h"

#define MAX(a,b) (((a)>(b))?(a):(b))
#define MIN(a,b) (((a)<(b))?(a):(b))

#define MAX_COWORKERS 32

typedef enum
{
    LEFT = 0,
    UP = 1,
    RIGHT = 2,
    DOWN = 4
} Direction;

typedef struct
{
    Rectangle source;
    Vector2 pos;
    Color tint;
    int layer;
} DrawJob;

#endif
