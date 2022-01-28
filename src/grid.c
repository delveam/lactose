#include <stdio.h>
#include <string.h>
#include "player.h"
#include "grid.h"

static Cell map[GRID_WIDTH * GRID_HEIGHT];
static Fart zone[GRID_WIDTH * GRID_HEIGHT];
static Fart zoneBackBuffer[GRID_WIDTH * GRID_HEIGHT];
static int turns = 0;
static float timer = 0;

static Player player;
static float fartFrequency;

void InitWorld()
{
    Cell mapTemp[] =
    {
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
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
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
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

    memcpy(map, mapTemp, sizeof(Cell) * GRID_WIDTH * GRID_HEIGHT);
    memcpy(zone, zoneTmp, sizeof(Fart) * GRID_WIDTH * GRID_HEIGHT);

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
                    {
                        SetFart(zoneBackBuffer, x, y, SMALL);

                        if (GetFart(zoneBackBuffer, x, y - 1) == SMALL)
                        {
                            SetFart(zoneBackBuffer, x, y - 1, BIG);
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
                    Color color;
                    color.g = 100;
                    color.a = 255;
                    DrawRectangle(x * GRID_SIZE, y * GRID_SIZE, GRID_SIZE, GRID_SIZE, color);
                    break;
                }
                case SMALL:
                {
                    Color color;
                    color.g = 200;
                    color.a = 255;
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
            if (map[y * GRID_WIDTH + x] == WALL)
            {
                DrawRectangle(x * GRID_SIZE, y * GRID_SIZE, GRID_SIZE, GRID_SIZE, BLACK);
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

Fart GetFart(Fart* self, int x, int y)
{
    if (x >= GRID_WIDTH || x < 0 || y >= GRID_HEIGHT || y < 0)
    {
        return AIR;
    }

    return self[y * GRID_WIDTH + x];
}

void SetFart(Fart* help, int x, int y, Fart value)
{
    if (x >= GRID_WIDTH || x < 0 || y >= GRID_HEIGHT || y < 0)
    {
        return;
    }

    help[y * GRID_WIDTH + x] = value;
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
