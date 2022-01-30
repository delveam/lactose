#include "coworker.h"

#include <assert.h>

const static Direction turnSequence[4] = { LEFT, DOWN, RIGHT, DOWN };

void CoworkerInit(Coworker* coworker, float moveDuration, float waitDuration, Coord startPos,
                  Coord endPos, float size, int isMan)
{
    coworker->facing = DOWN;
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

    coworker->facing = DOWN;
    coworker->isMan = isMan;
    coworker->fartStunned = 0;
    coworker->stunTimer = 0.0f;
    coworker->turnIndex = 0;
    coworker->exclaimIndex = 0;
    coworker->health = 24;

    if (dx != 0)
    {
        coworker->walkDirection = HORIZONTAL;
    }
    else if (dy != 0)
    {
        coworker->walkDirection = VERTICAL;
    }
    else
    {
        coworker->walkDirection = IDLE;
    }
}

static void Move(Coworker* coworker, int dx, int dy)
{
    switch (coworker->walkDirection)
    {
        case VERTICAL:
        {
            if (dy > 0)
            {
                coworker->facing = DOWN;
            }
            else if (dy < 0)
            {
                coworker->facing = UP;
            }
            coworker->pos.y += dy;
            break;
        }
        case HORIZONTAL:
        {
            if (dx < 0)
            {
                coworker->facing = LEFT;
            }
            else if (dx > 0)
            {
                coworker->facing = RIGHT;
            }
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
    if (coworker->moveTimer <= coworker->moveDuration)
    {
        return;
    }

    switch (coworker->walkDirection)
    {
        case HORIZONTAL:
        {
            int dx = 0;
            if ((coworker->tgtPos.x - coworker->pos.x) > 0)
            {
                dx = 1;
            }
            else
            {
                dx = -1;
            }
            Move(coworker, dx, 0);
            break;
        }
        case VERTICAL:
        {
            int dy = 0;
            if ((coworker->tgtPos.y - coworker->pos.y) > 0)
            {
                dy = 1;
            }
            else
            {
                dy = -1;
            }
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
    if (coworker->waitTimer > 0.5)
    {
        coworker->facing = DOWN;
    }

    if (coworker->waitTimer > coworker->waitDuration)
    {
        coworker->moveTimer = 0;
        coworker->waitTimer = -1;
    }
}

static void FartInteraction(Coworker* coworker)
{
    coworker->fartStunned = InFartZone(coworker->pos.x, coworker->pos.y);

    if (!coworker->fartStunned)
    {
        coworker->stunTimer = 0;
        return;
    }

    coworker->stunTimer += GetFrameTime();

    if (coworker->stunTimer < 0.2f)
    {
        return;
    }

    coworker->health -= 1;

    coworker->turnIndex += 1;

    if (coworker->turnIndex >= 4)
    {
        coworker->turnIndex = 0;
    }

    coworker->exclaimIndex += 1;

    if (coworker->exclaimIndex >= 2)
    {
        coworker->exclaimIndex = 0;
    }

    coworker->stunTimer = 0;

    // switch (coworker->facing)
    // {
    //     case LEFT:
    //     {
    //         coworker->facing = UP;
    //         break;
    //     }
    //     case UP:
    //     {
    //         coworker->facing = RIGHT;
    //         break;
    //     }
    //     case RIGHT:
    //     {
    //         coworker->facing = DOWN;
    //         break;
    //     }
    //     case DOWN:
    //     {
    //         coworker->facing = LEFT;
    //         break;
    //     }
    // }
    coworker->facing = turnSequence[coworker->turnIndex];
}

void CoworkerUpdate(Coworker* coworker)
{
    if (coworker->health <= 0)
    {
        return;
    }

    FartInteraction(coworker);

    if (coworker->fartStunned)
    {
        return;
    }

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

void CoworkerRegisterDraw(Coworker* coworker)
{
    if (coworker->health <= 0)
    {
        Color color = { 255, 255, 255, 255 };
        Rectangle source = { 32, 128, 48, 32 };

        if (!coworker->isMan)
        {
            source.x = 208;
            source.y = 96;
        }

        Vector2 position = { coworker->pos.x * GRID_SIZE - 10, coworker->pos.y * GRID_SIZE - 35 + 3 + GRID_SIZE * 1 + 8 };
        DrawJob job = { source, position, color, 2 };
        RegisterDraw(job);

        return;
    }

    {
        Rectangle source = { 0, 208, 32, 48 };

        switch (coworker->facing)
        {
            case LEFT:
            {
                source.x = 0 + source.width * 0;
                break;
            }
            case UP:
            {
                source.x = 0 + source.width * 1;
                break;
            }
            case RIGHT:
            {
                source.x = 0 + source.width * 2;
                break;
            }
            case DOWN:
            {
                source.x = 0 + source.width * 3;
                break;
            }
        }

        if (!coworker->isMan)
        {
            source.x += 128;
        }

        {
            Vector2 position = { coworker->pos.x * GRID_SIZE - 10 + 3, coworker->pos.y * GRID_SIZE - 35 + 3 };
            DrawJob job = { source, position, WHITE, 2 };
            RegisterDraw(job);
        }

        if (InFartZone(coworker->pos.x, coworker->pos.y))
        {
            Rectangle src = { 96 + 32 * coworker->exclaimIndex, 128, 32, 32 };

            Vector2 position = { coworker->pos.x * GRID_SIZE - 8, coworker->pos.y * GRID_SIZE - GRID_SIZE * 2.5f };
            DrawJob job = { src, position, WHITE, 5 };
            RegisterDraw(job);
        }
    }
}
