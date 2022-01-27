#ifndef PLAYER_H
#define PLAYER_H

#include "grid.h"
#include "raylib.h"

typedef struct
{
    Coord pos;
    Rectangle rect;
} Player;

void PlayerInit(Player* player, int x, int y, float size);
void PlayerMove(Player* player, int x, int y);
void PlayerUpdate(Player* player);
void PlayerDraw(Player* player);
void PlayerDebug(Player* player);

#endif