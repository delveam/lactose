#include "coworker.h"

#include <assert.h>

void CoworkerInit(Coworker* coworker, float moveDuration, float waitDuration, Coord startPos, Coord endPos, float size)
{
    coworker->direction = DOWN;
    coworker->pos = startPos;
    coworker->startPos = startPos;
    coworker->endPos = endPos;
    coworker->tgtPos = endPos;

    coworker->size = size;

    coworker->rect.width = size;
    coworker->rect.height = size;

    Vector2 rectPos = GridToScreen(startPos.x, startPos.y);
    coworker->rect.x = rectPos.x - size * 0.5;
    coworker->rect.y = rectPos.y - size * 0.5;

    coworker->moveDuration = moveDuration;
    coworker->waitDuration = waitDuration;

    coworker->moveTimer = 0;
    coworker->waitTimer = -1;
    
    int dx = startPos.x - endPos.x;
    int dy = startPos.y - endPos.y;

    coworker->direction = DOWN;

    if (dx != 0) coworker->walkDirection = HORIZONTAL;
    else if (dy != 0) coworker->walkDirection = VERTICAL;
    else coworker->walkDirection = IDLE;
}

static void Move(Coworker* coworker, int dx, int dy)
{
    switch (coworker->walkDirection)
    {
        case VERTICAL:
        {
            if (dy > 0) coworker->direction = UP;
            else if (dy < 0) coworker->direction = DOWN;
            coworker->pos.y += dy;
            break;
        }
        case HORIZONTAL:
        {
            if (dx < 0) coworker->direction = LEFT;
            else if (dx > 0) coworker->direction = RIGHT;
            coworker->pos.x += dx;
            break;
        }
        default:
            break;
    }

    Vector2 screenPos = GridToScreen(coworker->pos.x, coworker->pos.y);
    coworker->rect.x = screenPos.x - coworker->size * 0.5;
    coworker->rect.y = screenPos.y - coworker->size * 0.5;
}

static void WalkUpdate(Coworker* coworker)
{
    if (coworker->moveTimer <= coworker->moveDuration) return;

    switch (coworker->walkDirection)
    {
        case HORIZONTAL:
        {
            int dx = 0;
            if ((coworker->tgtPos.x - coworker->pos.x) > 0) dx = 1;
            else dx = -1;
            Move(coworker, dx, 0);
            break;
        }
        case VERTICAL:
        {
            int dy = 0;
            if ((coworker->tgtPos.y - coworker->pos.y) > 0) dy = 1;
            else dy = -1;
            Move(coworker, 0, dy);
            break;
        }
        default:
            break;
    }

    coworker->moveTimer = 0;

    if (coworker->pos.x == coworker->tgtPos.x && coworker->pos.y == coworker->tgtPos.y)
    {
        coworker->moveTimer = -1;
        coworker->waitTimer = 0;
        coworker->direction = DOWN;
        if (coworker->pos.x == coworker->endPos.x && coworker->pos.y == coworker->endPos.y)
        {
            // at the endPos
            coworker->tgtPos = coworker->startPos;
        }
        else
        {
            // at startPos
            coworker->tgtPos = coworker->endPos;
        }
    }
}

static void WaitUpdate(Coworker* coworker)
{
    if (coworker->waitTimer > coworker->waitDuration)
    {
        coworker->moveTimer = 0;
        coworker->waitTimer = -1;
    }
}

void CoworkerUpdate(Coworker* coworker)
{
    // timers should be mutually exclusive
    assert(coworker->moveTimer != -1 ^ coworker->waitTimer != -1);
    
    int isWaiting = coworker->moveTimer == -1;
    int isWalking = coworker->waitTimer == -1;

    if (isWalking)
    {
        coworker->moveTimer += GetFrameTime();
        WalkUpdate(coworker);
    }
    else if (isWaiting)
    {
        coworker->waitTimer += GetFrameTime();
        WaitUpdate(coworker);
    }
}

void CoworkerDraw(Coworker* coworker)
{
    DrawRectangleRec(coworker->rect, RED);
}