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

void InitWorld();
Cell GetCell(int x, int y);
Coord ScreenToGrid(float x, float y);
Vector2 GridToScreen(int x, int y);
void DrawWorld();

#endif