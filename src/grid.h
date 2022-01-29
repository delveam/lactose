#ifndef GRID_H
#define GRID_H

#include "raylib.h"
#include "constants.h"

#define GRID_WIDTH 32
#define GRID_HEIGHT 32
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
    WALL = 2,
    PICKUP = 4,
    DROPOFF = 5
} Cell;

typedef enum
{
    AIR = 0,
    SMALL = 1,
    BIG = 2,
} Fart;

void InitWorld();
void DestroyWorld();
Cell GetCell(int x, int y);
Cell GetTask(int x, int y);
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
void IncrementTurns();
void RegisterDraw(DrawJob job);

#endif
