#ifndef PLAYER_H
#define PLAYER_H

#include "constants.h"
#include "grid.h"
#include "raylib.h"

typedef enum
{
    EMPTY = 0,
    PARCEL = 1,
} Item;

typedef struct
{
    Coord pos;
    Rectangle rect;
    Direction facing;
    Item holding;
    Coord pickUpCoord;
    int holdingId;
} Player;

void PlayerInit(Player* player, int x, int y, float size);
void PlayerMove(Player* player, Direction direction);
void PlayerUpdate(Player* player);
void PlayerDraw(Player* player, Texture2D sprites);
void PlayerDebug(Player* player);

#endif
