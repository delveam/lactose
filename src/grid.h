#ifndef GRID_H
#define GRID_H

#include "raylib.h"

#define GRID_WIDTH 16
#define GRID_HEIGHT 16
#define GRID_AREA GRID_WIDTH * GRID_HEIGHT
#define GRID_SIZE 16

typedef struct
{
    int x;
    int y;
} Coord;

typedef enum
{
    NONE = 0,
    WALL = 1,
    PICKUP = 2,
    DROPOFF = 3
} Cell;

typedef enum
{
    AIR = 0,
    SMALL = 1,
    BIG = 2,
} Fart;

void InitWorld();
Cell GetCell(int x, int y);
Fart GetFart(Fart* self, int x, int y);
void SetFart(Fart* help, int x, int y, Fart value);
Coord ScreenToGrid(float x, float y);
Vector2 GridToScreen(int x, int y);
void FartHere(int x, int y);
int PickUpLookUp(int x, int y);
int GetPickUpCount(int x, int y);
void DecrementPickUpCount(int x, int y);
void UpdateWorld();
void DrawWorld();

#endif
