#ifndef CONSTANTS_H
#define CONSTANTS_H

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

#endif
