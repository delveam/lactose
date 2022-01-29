#include <stdio.h>
#include "player.h"

static Coord GetFront(Player* player)
{
    // WARNING: Can return out of bounds!

    Coord result = player->pos;

    switch (player->facing)
    {
        case LEFT:
        {
            result.x -= 1;
            break;
        }
        case UP:
        {
            result.y -= 1;
            break;
        }
        case RIGHT:
        {
            result.x += 1;
            break;
        }
        case DOWN:
        {
            result.y += 1;
            break;
        }
    }

    return result;
}

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
    player->holding = EMPTY;
    player->pickUpCoord.x = -1;
    player->pickUpCoord.y = -1;
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

    IncrementTurns();
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

    if (IsKeyPressed(KEY_SPACE))
    {
        Coord front = GetFront(player);
        Cell frontCell = GetCell(front.x, front.y);

        if (player->holding != EMPTY)
        {
            // Player is holding something
            if (frontCell == DROPOFF)
            {
                // Player in front of drop off
                if (player->holdingId == PickUpLookUp(front.x, front.y))
                {
                    // Player has successfully dropped off the parcel.
                    player->holding = EMPTY;
                    player->holdingId = 0;

                    DecrementPickUpCount(player->pickUpCoord.x, player->pickUpCoord.y);
                }
            }
            else if (frontCell != PICKUP)
            {
                // Player not in front of pickup
                // TODO: ideally, would throw item in front, but for now throw into void
                player->holding = EMPTY;
                player->holdingId = 0;
            }
        }
        else if (frontCell == PICKUP && GetPickUpCount(front.x, front.y) > 0)
        {
            // Player is not holding something and there is an item to pickup
            player->holding = PARCEL;
            player->holdingId = PickUpLookUp(front.x, front.y);
            player->pickUpCoord.x = front.x;
            player->pickUpCoord.y = front.y;
        }
    }
}

void PlayerUpdate(Player* player)
{
    Input(player);
}

void PlayerDraw(Player* player, Texture2D sprites)
{
    // (10, 35), 12x12
    {
        Rectangle source = { 0, 160, 32, 48 };

        switch (player->facing)
        {
            case LEFT:
            {
                source.x = 0 + source.width * 0;
                break;
            }
            case UP:
            {
                source.x = 0 + source.width * 1;
                break;
            }
            case RIGHT:
            {
                source.x = 0 + source.width * 2;
                break;
            }
            case DOWN:
            {
                source.x = 0 + source.width * 3;
                break;
            }
        }

        Vector2 position = { player->pos.x * GRID_SIZE - 10, player->pos.y * GRID_SIZE - 35 };
        DrawTextureRec(sprites, source, position, WHITE);
    }

    // DrawRectangleRec(player->rect, RED);

    // Rectangle tmp = player->rect;
    //
    // switch (player->facing)
    // {
    //     case LEFT:
    //     {
    //         tmp.x -= GRID_SIZE;
    //         break;
    //     }
    //     case UP:
    //     {
    //         tmp.y -= GRID_SIZE;
    //         break;
    //     }
    //     case RIGHT:
    //     {
    //         tmp.x += GRID_SIZE;
    //         break;
    //     }
    //     case DOWN:
    //     {
    //         tmp.y += GRID_SIZE;
    //         break;
    //     }
    // }
    //
    // DrawRectangleRec(tmp, BLUE);
    //
    // if (player->holding)
    // {
    //     DrawText("HOLDING", 8, 8, 8, BLACK);
    // }
}

void PlayerDebug(Player* player)
{
    // printf("(%d, %d)\n", player->pos.x, player->pos.y);
    printf("%d\n", player->holding);
}
