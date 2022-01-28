#ifndef GRID_H
#define GRID_H

#include "raylib.h"

#define GRID_WIDTH 16
#define GRID_HEIGHT 16
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
} Cell;

typedef enum
{
    // TODO(thismarvin): Do we need to prefix this?
    // NONE = 0,
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
void UpdateWorld();
void DrawWorld();

#endif
