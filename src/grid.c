#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "player.h"
#include "grid.h"

static Cell map[GRID_AREA];
static Fart zone[GRID_AREA];
static Fart zoneBackBuffer[GRID_AREA];
static int pickUpIds[GRID_AREA];
static int pickUpCounts[GRID_AREA];
static int turns = 0;
static float timer = 0;

static Player player;
static float fartFrequency;

void InitWorld()
{
    Cell mapTemp[] =
    {
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 2, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    };

    Fart zoneTmp[] =
    {
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 2, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    };

    int pickUpIdsTmp[] =
    {
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 2, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    };

    int pickUpCountsTmp[] =
    {
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    };

    memcpy(map, mapTemp, sizeof(Cell) * GRID_AREA);
    memcpy(zone, zoneTmp, sizeof(Fart) * GRID_AREA);
    memcpy(zoneBackBuffer, zoneTmp, sizeof(Fart) * GRID_AREA);
    memcpy(pickUpIds, pickUpIdsTmp, sizeof(int) * GRID_AREA);
    memcpy(pickUpCounts, pickUpCountsTmp, sizeof(int) * GRID_AREA);

    turns = 0;
    timer = 0.0f;

    PlayerInit(&player, 2, 2, 8);
    fartFrequency = 0.0f;
}

void FartHere(int x, int y)
{
    zone[y * GRID_WIDTH + x] = BIG;
}

static void CleanUpSmallFarts()
{
    for (int y = 0; y < GRID_HEIGHT; ++y)
    {
        for (int x = 0; x < GRID_WIDTH; ++x)
        {
            if (GetFart(zone, x, y) == SMALL)
            {
                SetFart(zone, x, y, AIR);
            }
        }
    }

}

static int DeliveredAll()
{
    int sum = 0;

    for (int i = 0; i < GRID_AREA; ++i)
    {
        sum += pickUpCounts[i];
    }

    return !sum;
}

static void ExpandBigFart(Fart* targetZone, int x, int y)
{
    assert(GetFart(targetZone, x, y) == BIG);

    Fart topFart = GetFart(targetZone, x, y - 1);
    Fart botFart = GetFart(targetZone, x, y + 1);
    Fart leftFart = GetFart(targetZone, x - 1, y);
    Fart rightFart = GetFart(targetZone, x + 1, y);

    // top
    if (topFart == SMALL)
    {
        SetFart(targetZone, x, y - 1, BIG);
    }
    else if (topFart == AIR)
    {
        SetFart(targetZone, x, y - 1, SMALL);
    }

    // bottom
    if (botFart == SMALL)
    {
        SetFart(targetZone, x, y + 1, BIG);
    }
    else if (botFart == AIR)
    {
        SetFart(targetZone, x, y + 1, SMALL);
    }

    // left
    if (leftFart == SMALL)
    {
        SetFart(targetZone, x - 1, y, BIG);
    }
    else if (leftFart == AIR)
    {
        SetFart(targetZone, x - 1, y, SMALL);
    }

    // right
    if (rightFart == SMALL)
    {
        SetFart(targetZone, x + 1, y, BIG);
    }
    else if (rightFart == AIR)
    {
        SetFart(targetZone, x + 1, y, SMALL);
    }

    SetFart(targetZone, x, y, SMALL);
}

static void ProcessBigFarts()
{
    for (int y = 0; y < GRID_HEIGHT; ++y)
    {
        for (int x = 0; x < GRID_WIDTH; ++x)
        {
            if (GetFart(zoneBackBuffer, x, y) == BIG)
            {
                ExpandBigFart(zone, x, y);
            }
        }
    }

    memcpy(zoneBackBuffer, zone, sizeof(Fart) * GRID_AREA);
}

void UpdateWorld()
{
    // TODO(thismarvin): `turns` property
    // TODO(thismarvin): Celluar Automaton... Fart physics

    PlayerUpdate(&player);
    // PlayerDebug(&player);


    if (DeliveredAll())
    {
        // TODO(thismarvin): Acknowledge that the player has finished their job!
    }

    timer += GetFrameTime();
    fartFrequency += GetFrameTime();

    if (timer >= 2.0f)
    {
        CleanUpSmallFarts();
        ProcessBigFarts();

        timer = 0;
    }

    // TODO: move this in player
    if (fartFrequency >= 1.0f)
    {
        int dx = 0;
        int dy = 0;

        switch (player.facing)
        {
            case LEFT:
            {
                dx = 1;
                break;
            }
            case UP:
            {
                dy = 1;
                break;
            }
            case RIGHT:
            {
                dx = -1;
                break;
            }
            case DOWN:
            {
                dy = -1;
                break;
            }
        }

        SetFart(zone, player.pos.x + dx, player.pos.y + dy, BIG);

        fartFrequency = 0;
    }
}

void DrawWorld()
{
    for (int y = 0; y < GRID_HEIGHT; ++y)
    {
        for (int x = 0; x < GRID_WIDTH; ++x)
        {
            switch (zone[y * GRID_WIDTH + x])
            {
                case BIG:
                {
                    Color color = { 0, 100, 0, 255 };
                    DrawRectangle(x * GRID_SIZE, y * GRID_SIZE, GRID_SIZE, GRID_SIZE, color);
                    break;
                }
                case SMALL:
                {
                    Color color = { 0, 200, 0, 255 };
                    DrawRectangle(x * GRID_SIZE, y * GRID_SIZE, GRID_SIZE, GRID_SIZE, color);
                    break;
                }
                default:
                    break;
            }
        }
    }

    for (int y = 0; y < GRID_HEIGHT; ++y)
    {
        for (int x = 0; x < GRID_WIDTH; ++x)
        {
            switch (map[y * GRID_WIDTH + x])
            {
                case WALL:
                    DrawRectangle(x * GRID_SIZE, y * GRID_SIZE, GRID_SIZE, GRID_SIZE, BLACK);
                    break;
                case DROPOFF:
                    DrawRectangle(x * GRID_SIZE, y * GRID_SIZE, GRID_SIZE, GRID_SIZE, RED);
                    break;
                case PICKUP:
                    DrawRectangle(x * GRID_SIZE, y * GRID_SIZE, GRID_SIZE, GRID_SIZE, ORANGE);
                    break;
                default:
                    break;
            }
        }
    }

    PlayerDraw(&player);
}

Cell GetCell(int x, int y)
{
    if (x >= GRID_WIDTH || x < 0 || y >= GRID_HEIGHT || y < 0)
    {
        return NONE;
    }

    return map[y * GRID_WIDTH + x];
}

Fart GetFart(Fart* targetZone, int x, int y)
{
    if (x >= GRID_WIDTH || x < 0 || y >= GRID_HEIGHT || y < 0)
    {
        return AIR;
    }

    return targetZone[y * GRID_WIDTH + x];
}

void SetFart(Fart* targetZone, int x, int y, Fart value)
{
    if (x >= GRID_WIDTH || x < 0 || y >= GRID_HEIGHT || y < 0)
    {
        return;
    }

    targetZone[y * GRID_WIDTH + x] = value;
}

Coord ScreenToGrid(float x, float y)
{
    int xGrid = (int)x / GRID_SIZE;
    int yGrid = (int)y / GRID_SIZE;

    Coord result = { xGrid, yGrid };
    return result;
}

Vector2 GridToScreen(int x, int y)
{
    float xScreen = x * GRID_SIZE + GRID_SIZE / 2;
    float yScreen = y * GRID_SIZE + GRID_SIZE / 2;

    Vector2 result = { xScreen, yScreen };
    return result;
}

int PickUpLookUp(int x, int y)
{
    return pickUpIds[y * GRID_WIDTH + x];
}

int GetPickUpCount(int x, int y)
{
    return pickUpCounts[y * GRID_WIDTH + x];
}

void DecrementPickUpCount(int x, int y)
{
    int tmp = pickUpCounts[y * GRID_WIDTH + x] - 1;

    tmp = MAX(0, tmp);

    printf("%d\n", tmp);

    pickUpCounts[y * GRID_WIDTH + x] = tmp;
}