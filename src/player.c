#include <stdio.h>
#include "player.h"

void PlayerInit(Player* player, int x, int y, float size)
{
    player->pos.x = x;
    player->pos.y = y;

    player->rect.width = size;
    player->rect.height = size;

    Vector2 rectPos = GridToScreen(x, y);
    player->rect.x = rectPos.x - size * 0.5;
    player->rect.y = rectPos.y - size * 0.5;
}

void PlayerMove(Player* player, int dx, int dy)
{
    int newX = player->pos.x + dx;
    int newY = player->pos.y + dy;

    if (newX >= GRID_WIDTH || newX < 0 ||
            newY >= GRID_HEIGHT || newY < 0 ||
            GetCell(newX, newY) == WALL)
    {
        return;
    }

    player->pos.x = newX;
    player->pos.y = newY;

    Vector2 newRectPos = GridToScreen(newX, newY);
    player->rect.x = newRectPos.x - player->rect.width * 0.5;
    player->rect.y = newRectPos.y - player->rect.height * 0.5;
}

static void Input(Player* player)
{
    if (IsKeyPressed(KEY_UP))
    {
        PlayerMove(player, 0, -1);
    }
    else if (IsKeyPressed(KEY_DOWN))
    {
        PlayerMove(player, 0, 1);
    }
    else if (IsKeyPressed(KEY_LEFT))
    {
        PlayerMove(player, -1, 0);
    }
    else if (IsKeyPressed(KEY_RIGHT))
    {
        PlayerMove(player, 1, 0);
    }
}

void PlayerUpdate(Player* player)
{
    Input(player);
}

void PlayerDraw(Player* player)
{
    DrawRectangleRec(player->rect, RED);
}

void PlayerDebug(Player* player)
{
    printf("(%d, %d)\n", player->pos.x, player->pos.y);
}