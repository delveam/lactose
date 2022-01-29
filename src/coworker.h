#ifndef COWORKER_H
#define COWORKER_H

#include "grid.h"
#include "constants.h"

typedef enum
{
    IDLE = 0,
    HORIZONTAL = 1,
    VERTICAL = 2
} WalkDir;

// For the timers, -1 indicates it is not in the state associated with the timer (e.g. not currently moving).
typedef struct {
    Coord pos;
    Coord startPos;
    Coord endPos;
    Coord tgtPos;
    Rectangle rect;
    float size;
    float moveDuration;
    float moveTimer; // acts as a movement speed
    float waitDuration;
    float waitTimer;
    Direction facing;
    WalkDir walkDirection;
    int isMan;
} Coworker;

void CoworkerInit(Coworker* coworker, float moveDuration, float waitDuration, Coord startPos, Coord endPos, float size, int isMan);
void CoworkerUpdate(Coworker* coworker);
void CoworkerDraw(Coworker* coworker, Texture2D sprites);


#endif