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

    player->facing = DOWN;
}

void PlayerMove(Player* player, Direction direction)
{
    int dx = 0;
    int dy = 0;

    switch (direction)
    {
        case LEFT:
        {
            dx = -1;
            break;
        }
        case UP:
        {
            dy = -1;
            break;
        }
        case RIGHT:
        {
            dx = 1;
            break;
        }
        case DOWN:
        {
            dy = 1;
            break;
        }
    }

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

    player->facing = direction;
}

static void Input(Player* player)
{
    if (IsKeyPressed(KEY_UP))
    {
        PlayerMove(player, UP);
    }
    else if (IsKeyPressed(KEY_DOWN))
    {
        PlayerMove(player, DOWN);
    }
    else if (IsKeyPressed(KEY_LEFT))
    {
        PlayerMove(player, LEFT);
    }
    else if (IsKeyPressed(KEY_RIGHT))
    {
        PlayerMove(player, RIGHT);
    }
}

void PlayerUpdate(Player* player)
{
    Input(player);
}

void PlayerDraw(Player* player)
{
    DrawRectangleRec(player->rect, RED);

    Rectangle tmp = player->rect;

    switch (player->facing)
    {
        case LEFT:
        {
            tmp.x -= GRID_SIZE;
            break;
        }
        case UP:
        {
            tmp.y -= GRID_SIZE;
            break;
        }
        case RIGHT:
        {
            tmp.x += GRID_SIZE;
            break;
        }
        case DOWN:
        {
            tmp.y += GRID_SIZE;
            break;
        }
    }

    DrawRectangleRec(tmp, BLUE);
}

void PlayerDebug(Player* player)
{
    printf("(%d, %d)\n", player->pos.x, player->pos.y);
}
