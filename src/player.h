#ifndef PLAYER_H
#define PLAYER_H

#include "constants.h"
#include "grid.h"
#include "raylib.h"

typedef struct
{
    Coord pos;
    Rectangle rect;
    Direction facing;
} Player;

void PlayerInit(Player* player, int x, int y, float size);
void PlayerMove(Player* player, Direction direction);
void PlayerUpdate(Player* player);
void PlayerDraw(Player* player);
void PlayerDebug(Player* player);

#endif
