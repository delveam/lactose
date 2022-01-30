#include "grid.h"
#include "coworker.h"
#include "player.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// TODO(thismarvin): We do not need this anymore; remove it and use GRID_WIDTH/GRID_HEIGHT;
static const int mapWidth = 32;
static const int mapHeight = 32;

static const int atlasWidth = 16;
static const int spriteSize = 16;

// This is really just walls;
static Cell map[GRID_AREA];
static int wallSprites[GRID_AREA];
static int spritesFloor[GRID_AREA];
static int spritesDecoration[GRID_AREA];
static int spritesTasks[GRID_AREA];

static Cell tasks[GRID_AREA] = { 0 };
static int pickUpIds[GRID_AREA];
static int pickUpCounts[GRID_AREA];

static Fart zone[GRID_AREA] = { 0 };
static Fart zoneBackBuffer[GRID_AREA] = { 0 };
static int turns = 0;
static int turnNeedsToBeResolved = 0;
static float fartUpdateStopwatch = 0;

static Player player;
static Coworker coworkers[MAX_COWORKERS] = { 0 };
static int nextCoworkerIdx = 0;
static float fartOverloadStopwatch;
static float fartCooldownStopwatch;
static float fartFrequencyStopwatch;

static Camera2D camera;
static Texture2D sprites;
static DrawJob drawJobs[GRID_AREA * 3] = { 0 };
static int nextDrawJobIdx;

static Sound fartSounds[3];
static Music bgm;

static Sound fartSounds[3];
static Music bgm;

static void AddCoworker(Coord startPos, Coord endPos, int gender);

void InitWorld()
{
    {
        Vector2 offset = { 0, 0 };
        Vector2 target = { 0, 0 };

        camera.offset = offset;
        camera.target = target;
        camera.rotation = 0;
        camera.zoom = 3;
    }

    sprites = LoadTexture("src/resources/sprites.png");
    fartSounds[0] = LoadSound("src/resources/fart_0.mp3");
    fartSounds[1] = LoadSound("src/resources/fart_1.mp3");
    fartSounds[2] = LoadSound("src/resources/fart_2.mp3");
    bgm = LoadMusicStream("src/resources/bgm.mp3");

    // NOTE: The following line if only for debugging...
    // sprites = LoadTexture("resources/sprites.png");
    // fartSounds[0] = LoadSound("resources/fart_0.mp3");
    // fartSounds[1] = LoadSound("resources/fart_1.mp3");
    // fartSounds[2] = LoadSound("resources/fart_2.mp3");
    // bgm = LoadMusicStream("resources/bgm.wav");

    SetSoundVolume(fartSounds[0], 0.2f);
    SetSoundVolume(fartSounds[1], 0.2f);
    SetSoundVolume(fartSounds[2], 0.2f);

    SetMusicVolume(bgm, 0.05f);
    PlayMusicStream(bgm);

    {
        int tmp[] = { 34, 34, 34, 34, 34, 34, 34, 34, 34, 34, 34, 34, 34, 36, 36, 34, 34, 34, 34, 34, 34, 34, 34, 34, 34, 36, 37, 0, 0, 33, 34, 34, 51, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 52, 52, 51, 50, 50, 50, 50, 50, 51, 50, 50, 50, 52, 53, 0, 0, 49, 50, 51, 51, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 68, 68, 51, 66, 66, 66, 66, 66, 67, 66, 66, 66, 68, 69, 0, 0, 65, 66, 51, 51, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 51, 0, 0, 0, 0, 0, 83, 0, 0, 0, 0, 0, 0, 0, 0, 0, 51, 51, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 51, 0, 0, 0, 0, 0, 99, 0, 0, 0, 0, 0, 0, 0, 0, 0, 51, 51, 0, 0, 0, 0, 0, 0, 23, 24, 0, 0, 23, 24, 0, 0, 51, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 51, 51, 0, 0, 0, 0, 0, 0, 39, 40, 0, 0, 39, 40, 0, 0, 51, 0, 0, 0, 0, 0, 101, 36, 36, 36, 36, 36, 36, 36, 36, 36, 51, 51, 0, 0, 0, 0, 0, 0, 55, 56, 0, 0, 55, 56, 0, 0, 51, 0, 0, 0, 0, 0, 67, 52, 52, 52, 52, 52, 52, 52, 52, 52, 51, 51, 0, 0, 0, 0, 0, 0, 71, 72, 0, 0, 71, 72, 0, 0, 51, 0, 0, 0, 0, 0, 83, 68, 68, 68, 68, 68, 68, 68, 68, 68, 51, 51, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 51, 0, 0, 0, 0, 0, 99, 0, 0, 0, 0, 0, 0, 0, 0, 0, 51, 51, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 51, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 51, 35, 36, 37, 38, 0, 0, 0, 33, 34, 36, 36, 36, 36, 36, 36, 35, 36, 36, 36, 36, 36, 36, 36, 36, 37, 0, 0, 0, 33, 36, 34, 35, 51, 52, 53, 54, 0, 0, 0, 49, 50, 52, 52, 52, 52, 52, 52, 51, 52, 52, 52, 52, 52, 51, 52, 52, 53, 0, 0, 0, 49, 52, 50, 51, 51, 68, 69, 70, 0, 0, 0, 65, 66, 68, 68, 68, 68, 68, 68, 51, 68, 68, 68, 68, 68, 51, 68, 68, 69, 0, 0, 0, 65, 68, 66, 51, 67, 84, 85, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 67, 0, 0, 0, 0, 0, 67, 0, 0, 0, 0, 0, 0, 0, 0, 0, 67, 83, 100, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 83, 0, 0, 0, 0, 0, 83, 0, 0, 0, 0, 0, 0, 0, 0, 0, 83, 99, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 99, 0, 0, 0, 0, 0, 99, 0, 0, 0, 0, 0, 0, 0, 0, 0, 99, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 34, 34, 34, 34, 34, 34, 34, 34, 34, 34, 34, 34, 34, 34, 36, 37, 0, 0, 0, 0, 0, 33, 34, 34, 34, 34, 34, 34, 34, 34, 34, 36, 51, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 52, 53, 0, 0, 0, 0, 0, 49, 50, 50, 52, 50, 50, 50, 50, 50, 50, 51, 51, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 68, 69, 0, 0, 0, 0, 0, 65, 66, 66, 68, 66, 66, 66, 66, 66, 66, 51, 51, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 51, 51, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 51, 51, 0, 0, 23, 24, 0, 0, 23, 24, 0, 0, 23, 24, 0, 0, 3, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 51, 51, 0, 0, 39, 40, 0, 0, 39, 40, 0, 0, 39, 40, 0, 0, 19, 0, 0, 0, 0, 0, 19, 0, 0, 0, 0, 0, 0, 0, 0, 0, 51, 51, 0, 0, 55, 56, 0, 0, 55, 56, 0, 0, 55, 56, 0, 0, 19, 0, 0, 0, 0, 0, 19, 0, 0, 0, 0, 0, 0, 0, 0, 0, 51, 51, 0, 0, 71, 72, 0, 0, 71, 72, 0, 0, 71, 72, 0, 0, 19, 0, 0, 0, 0, 0, 19, 0, 0, 0, 0, 0, 0, 0, 0, 0, 51, 51, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 19, 0, 0, 0, 0, 0, 19, 0, 0, 0, 0, 0, 0, 0, 0, 0, 51 };
        memcpy(wallSprites, tmp, sizeof(int) * GRID_AREA);
    }

    {
        int tmp[] = { 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 119, 103, 103, 104, 104, 104, 104, 104, 104, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 119, 103, 103, 103, 103, 103, 103, 119, 103, 103, 103, 103, 119, 103, 103, 104, 104, 104, 104, 104, 103, 103, 103, 119, 119, 103, 103, 103, 119, 119, 119, 103, 103, 103, 103, 103, 103, 103, 103, 103, 119, 119, 119, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 119, 103, 119, 119, 103, 103, 103, 103, 103, 103, 103, 103, 119, 119, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 119, 103, 103, 103, 103, 119, 103, 103, 104, 104, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 119, 104, 104, 103, 103, 103, 103, 103, 119, 103, 103, 103, 103, 103, 104, 104, 104, 104, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 119, 103, 104, 104, 119, 103, 103, 103, 103, 103, 103, 103, 119, 119, 103, 104, 104, 104, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 104, 104, 104, 104, 103, 103, 103, 119, 119, 119, 119, 103, 119, 103, 104, 104, 103, 103, 103, 103, 103, 103, 103, 104, 104, 103, 103, 103, 103, 103, 103, 103, 103, 103, 104, 104, 103, 103, 103, 103, 119, 103, 103, 103, 103, 103, 103, 103, 119, 119, 119, 103, 103, 103, 104, 104, 104, 104, 104, 103, 103, 103, 103, 103, 103, 119, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 104, 104, 104, 104, 104, 103, 103, 103, 103, 103, 103, 103, 119, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 104, 104, 104, 103, 103, 103, 103, 103, 103, 103, 103, 119, 119, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 119, 103, 103, 119, 119, 119, 103, 103, 103, 103, 119, 103, 103, 103, 103, 119, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 119, 119, 103, 103, 103, 103, 103, 103, 103, 103, 119, 103, 103, 103, 103, 103, 103, 103, 119, 103, 103, 103, 103, 103, 103, 103, 103, 119, 119, 119, 103, 103, 103, 103, 103, 103, 103, 103, 119, 119, 103, 119, 119, 103, 103, 104, 104, 104, 103, 119, 119, 103, 103, 119, 103, 119, 103, 103, 103, 119, 119, 119, 104, 103, 103, 103, 103, 103, 103, 103, 119, 119, 103, 103, 103, 104, 104, 119, 119, 104, 104, 103, 103, 103, 103, 119, 103, 103, 119, 119, 119, 119, 119, 119, 104, 104, 104, 119, 119, 119, 103, 119, 119, 103, 103, 104, 104, 119, 104, 119, 104, 104, 104, 103, 103, 119, 103, 103, 103, 104, 104, 104, 103, 119, 119, 119, 103, 104, 104, 104, 119, 103, 103, 119, 119, 103, 103, 104, 104, 104, 104, 103, 103, 103, 103, 103, 119, 119, 119, 103, 104, 104, 119, 104, 104, 104, 104, 103, 103, 103, 104, 103, 103, 103, 103, 103, 103, 119, 119, 119, 103, 103, 103, 103, 103, 119, 119, 119, 119, 119, 119, 103, 104, 119, 104, 104, 104, 104, 103, 119, 103, 103, 104, 103, 119, 103, 119, 103, 103, 119, 119, 119, 119, 119, 103, 103, 103, 119, 119, 103, 103, 103, 103, 104, 104, 104, 104, 103, 103, 103, 103, 103, 119, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 104, 104, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 119, 119, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 119, 119, 119, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 119, 103, 103, 103, 103, 119, 119, 119, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 119, 119, 119, 103, 103, 104, 104, 104, 103, 103, 103, 103, 103, 103, 119, 119, 103, 104, 103, 103, 104, 104, 103, 119, 119, 103, 103, 119, 119, 103, 103, 103, 103, 103, 103, 103, 104, 104, 104, 104, 103, 103, 103, 119, 103, 103, 119, 119, 103, 104, 103, 103, 104, 104, 104, 104, 103, 119, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 104, 104, 104, 104, 103, 103, 103, 119, 103, 103, 119, 119, 104, 104, 103, 103, 103, 104, 104, 104, 104, 104, 104, 103, 103, 103, 103, 119, 103, 103, 103, 103, 103, 104, 104, 104, 103, 103, 103, 119, 103, 103, 119, 119, 119, 104, 103, 103, 103, 104, 119, 119, 119, 104, 104, 104, 103, 103, 103, 103, 104, 104, 104, 104, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 104, 104, 103, 103, 119, 103, 103, 104, 104, 104, 104, 103, 103, 103, 103, 103, 104, 104, 104, 104, 103, 103, 103, 103, 119, 119, 119, 119, 119, 103, 103, 103, 104, 103, 103, 103, 119, 119, 103, 103, 103, 119, 119, 119, 103, 103 };
        memcpy(spritesFloor, tmp, sizeof(int) * GRID_AREA);
    }

    {
        int tmp[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 31, 32, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 97, 97, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 30, 0, 30, 30, 30, 0, 30, 0, 0, 0, 0, 0, 30, 30, 30, 0, 0, 113, 114, 116, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 46, 0, 46, 46, 46, 0, 46, 0, 60, 61, 62, 0, 46, 46, 46, 0, 0, 129, 130, 97, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 76, 77, 78, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 139, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 155, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 30, 0, 0, 0, 0, 0, 30, 30, 30, 0, 0, 0, 0, 0, 0, 0, 0, 0, 171, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 46, 0, 0, 0, 0, 0, 46, 46, 46, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 139, 0, 0, 0, 0, 0, 0, 0, 139, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 30, 30, 30, 0, 30, 0, 0, 155, 0, 60, 61, 62, 30, 30, 0, 155, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 46, 46, 46, 0, 46, 0, 0, 171, 0, 76, 77, 78, 46, 46, 0, 171, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 60, 61, 62, 0, 0, 60, 61, 62, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 156, 157, 158, 159, 0, 0, 0, 0, 76, 77, 78, 0, 0, 76, 77, 78, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 172, 173, 174, 175, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 60, 61, 62, 0, 0, 60, 61, 62, 0, 0, 0, 0, 0, 0, 156, 157, 158, 159, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 76, 77, 78, 0, 0, 76, 77, 78, 0, 0, 0, 0, 0, 0, 172, 173, 174, 175, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 30, 30, 30, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 30, 0, 30, 30, 0, 0, 0, 0, 0, 0, 46, 46, 46, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 46, 0, 46, 46, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 139, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 60, 61, 62, 0, 60, 61, 62, 0, 0, 0, 155, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 139, 0, 76, 77, 78, 0, 76, 77, 78, 0, 0, 0, 171, 0, 0, 0, 0, 0, 0, 0, 0, 0, 30, 0, 0, 0, 0, 0, 0, 0, 0, 0, 155, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 46, 0, 0, 0, 0, 0, 0, 0, 0, 0, 171, 0, 60, 61, 62, 0, 0, 0, 0, 0 };
        memcpy(spritesDecoration, tmp, sizeof(int) * GRID_AREA);
    }

    {
        int tmp[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 28, 29, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 44, 45, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 26, 27, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 42, 43, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
        memcpy(spritesTasks, tmp, sizeof(int) * GRID_AREA);
    }

    {
        int tmp[] = { 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 0, 2, 2, 0, 0, 0, 0, 0, 0, 2, 0, 2, 2, 2, 2, 2, 0, 2, 2, 2, 2, 2, 2, 2, 0, 0, 2, 2, 2, 0, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 2, 2, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 0, 0, 0, 0, 0, 0, 2, 2, 0, 0, 2, 2, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 0, 0, 0, 0, 0, 0, 2, 2, 0, 0, 2, 2, 0, 0, 2, 0, 0, 0, 0, 0, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 0, 0, 0, 0, 0, 0, 2, 2, 0, 0, 2, 2, 0, 0, 2, 0, 0, 0, 0, 0, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 0, 0, 0, 0, 0, 0, 2, 2, 0, 0, 2, 2, 0, 0, 2, 0, 0, 0, 0, 0, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 2, 2, 0, 0, 0, 0, 0, 2, 2, 2, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 2, 2, 0, 0, 0, 0, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 0, 0, 0, 2, 2, 2, 2, 2, 2, 2, 0, 0, 0, 0, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 0, 0, 0, 2, 2, 2, 2, 2, 2, 2, 0, 0, 0, 0, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 0, 0, 0, 2, 2, 2, 2, 2, 0, 0, 0, 0, 0, 0, 2, 2, 2, 0, 2, 0, 0, 2, 2, 2, 2, 2, 2, 2, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 0, 2, 2, 2, 0, 0, 2, 2, 2, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 2, 2, 2, 2, 0, 2, 0, 0, 2, 2, 2, 0, 0, 2, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 2, 0, 0, 2, 2, 2, 0, 0, 0, 0, 0, 0, 2, 2, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 2, 0, 0, 2, 2, 2, 0, 0, 0, 0, 0, 0, 2, 2, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 0, 0, 0, 0, 0, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 0, 0, 0, 0, 0, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 0, 0, 0, 0, 0, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 2, 2, 0, 2, 2, 0, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 0, 0, 2, 2, 0, 0, 2, 2, 0, 0, 2, 2, 0, 0, 2, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 0, 2, 2, 2, 0, 0, 2, 2, 0, 0, 2, 2, 0, 0, 2, 0, 0, 0, 0, 0, 2, 0, 0, 2, 2, 2, 0, 2, 2, 2, 2, 2, 0, 2, 2, 2, 0, 0, 2, 2, 0, 0, 2, 2, 0, 0, 2, 0, 0, 0, 0, 0, 2, 2, 0, 2, 2, 2, 0, 2, 2, 2, 2, 2, 0, 2, 2, 2, 0, 0, 2, 2, 0, 0, 2, 2, 0, 0, 2, 0, 0, 0, 0, 0, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 2, 0, 0, 0, 0, 0, 2, 2, 0, 2, 2, 2, 0, 0, 0, 0, 2 };
        memcpy(map, tmp, sizeof(int) * GRID_AREA);
    }

    {
        tasks[3 * GRID_WIDTH + 3] = PICKUP;
        tasks[29 * GRID_WIDTH + 24] = DROPOFF;
        pickUpIds[3 * GRID_WIDTH + 3] = 1;
        pickUpIds[28 * GRID_WIDTH + 25] = 1;
        pickUpCounts[3 * GRID_WIDTH + 3] = 3;
    }

    turns = 0;
    fartUpdateStopwatch = 0.0f;

    PlayerInit(&player, 2, 5, 8);
    fartOverloadStopwatch = 0.0f;
    fartCooldownStopwatch = 0.0f;
    fartFrequencyStopwatch = 0.0f;

    SetRandomSeed(time(0));

    {
        Coord startPos = { 5, 18 };
        Coord endPos = { 15, 18 };
        AddCoworker(startPos, endPos, 0);
    }

    {
        Coord startPos = { 10, 4 };
        Coord endPos = { 10, 9 };
        AddCoworker(startPos, endPos, 0);
    }

    {
        Coord startPos = { 19, 30 };
        Coord endPos = { 19, 16 };
        AddCoworker(startPos, endPos, 1);
    }

    {
        Coord startPos = { 2, 26 };
        Coord endPos = { 25, 26 };
        AddCoworker(startPos, endPos, 1);
    }

    {
        Coord pos = { 28, 19 };
        AddCoworker(pos, pos, GetRandomValue(0, 1));
    }

    {
        Coord pos = { 17, 3 };
        AddCoworker(pos, pos, GetRandomValue(0, 1));
    }
}

void RegisterDraw(DrawJob job)
{
    *(drawJobs + nextDrawJobIdx) = job;
    ++nextDrawJobIdx;
}

void DestroyWorld()
{
    UnloadTexture(sprites);

    for (int i = 0; i < 3; ++i)
    {
        UnloadSound(fartSounds[i]);
    }

    UnloadMusicStream(bgm);
}

void FartHere(int x, int y)
{
    zone[y * GRID_WIDTH + x] = BIG;
}

static void CleanUpSmallFarts()
{
    for (int y = 0; y < GRID_HEIGHT; ++y)
    {
        for (int x = 0; x < GRID_WIDTH; ++x)
        {
            if (GetFart(zone, x, y) == SMALL)
            {
                SetFart(zone, x, y, AIR);
            }
        }
    }

}

static int DeliveredAll()
{
    int sum = 0;

    for (int i = 0; i < GRID_AREA; ++i)
    {
        sum += pickUpCounts[i];
    }

    return !sum;
}

static void ExpandBigFart(Fart* targetZone, int x, int y)
{
    assert(GetFart(targetZone, x, y) == BIG);

    Fart topFart = GetFart(targetZone, x, y - 1);
    Fart botFart = GetFart(targetZone, x, y + 1);
    Fart leftFart = GetFart(targetZone, x - 1, y);
    Fart rightFart = GetFart(targetZone, x + 1, y);

    // top
    if (topFart == SMALL)
    {
        SetFart(targetZone, x, y - 1, BIG);
    }
    else if (topFart == AIR)
    {
        SetFart(targetZone, x, y - 1, SMALL);
    }

    // bottom
    if (botFart == SMALL)
    {
        SetFart(targetZone, x, y + 1, BIG);
    }
    else if (botFart == AIR)
    {
        SetFart(targetZone, x, y + 1, SMALL);
    }

    // left
    if (leftFart == SMALL)
    {
        SetFart(targetZone, x - 1, y, BIG);
    }
    else if (leftFart == AIR)
    {
        SetFart(targetZone, x - 1, y, SMALL);
    }

    // right
    if (rightFart == SMALL)
    {
        SetFart(targetZone, x + 1, y, BIG);
    }
    else if (rightFart == AIR)
    {
        SetFart(targetZone, x + 1, y, SMALL);
    }

    SetFart(targetZone, x, y, SMALL);
}

static void ProcessBigFarts()
{
    for (int y = 0; y < GRID_HEIGHT; ++y)
    {
        for (int x = 0; x < GRID_WIDTH; ++x)
        {
            if (GetFart(zoneBackBuffer, x, y) == BIG)
            {
                ExpandBigFart(zone, x, y);
            }
        }
    }

    memcpy(zoneBackBuffer, zone, sizeof(Fart) * GRID_AREA);
}

void UpdateWorld()
{
    UpdateMusicStream(bgm);

    PlayerUpdate(&player);

    // Update Camera
    {
        int viewportWidth = 16;
        int viewportHeight = 16;

        camera.offset.x = (-player.pos.x * GRID_SIZE + viewportWidth * 0.5 * GRID_SIZE) * camera.zoom;
        camera.offset.y = (-player.pos.y * GRID_SIZE + viewportHeight * 0.5 * GRID_SIZE) * camera.zoom;

        // printf("(%f, %f)\n", camera.offset.x, camera.offset.y);

        camera.offset.x = MIN(0, camera.offset.x);
        camera.offset.x = MAX((-(GRID_WIDTH - viewportWidth) * GRID_SIZE) * camera.zoom, camera.offset.x);

        camera.offset.y = MIN(0, camera.offset.y);
        camera.offset.y = MAX((-(GRID_HEIGHT - viewportHeight) * GRID_SIZE) * camera.zoom, camera.offset.y);
    }

    if (DeliveredAll())
    {
        // TODO(thismarvin): Acknowledge that the player has finished their job!
    }

    // Update Fart Physics.
    {
        fartUpdateStopwatch += GetFrameTime();

        if (fartUpdateStopwatch >= 2.0f)
        {
            CleanUpSmallFarts();
            ProcessBigFarts();

            fartUpdateStopwatch = 0;
        }
    }

    // Update Coworkers
    {
        for (int i = 0; i < nextCoworkerIdx; i++)
        {
            CoworkerUpdate(coworkers + i);
        }
    }

    // Update... Player's Fart Logic...
    {
        // TODO(thismarvin): Move this to player?
        fartOverloadStopwatch += GetFrameTime();

        if (fartOverloadStopwatch >= 5.0f)
        {
            fartCooldownStopwatch += GetFrameTime();
            if (fartCooldownStopwatch >= 6.0f)
            {
                fartOverloadStopwatch = 0.0f;
                fartCooldownStopwatch = 0.0f;
                fartFrequencyStopwatch = 0.0f;
            }

            fartFrequencyStopwatch += GetFrameTime();

            if (fartFrequencyStopwatch >= 1.5f || (turnNeedsToBeResolved && turns % 4 == 0))
            {
                int dx = 0;
                int dy = 0;

                switch (player.facing)
                {
                    case LEFT:
                        dx = 1;
                        break;
                    case UP:
                        dy = 1;
                        break;
                    case RIGHT:
                        dx = -1;
                        break;
                    case DOWN:
                        dy = -1;
                        break;
                }

                SetFart(zone, player.pos.x + dx, player.pos.y + dy, BIG);

                int index = GetRandomValue(0, 2);

                PlaySound(fartSounds[index]);

                fartFrequencyStopwatch = 0.0;
                turnNeedsToBeResolved = 0;
            }
        }
    }
}

static int DrawComp(const void* elem1, const void* elem2)
{
    DrawJob* job1 = ((DrawJob*) elem1);
    DrawJob* job2 = ((DrawJob*) elem2);
    float bot1 = job1->layer * GRID_HEIGHT * GRID_SIZE + (job1->pos.y + job1->source.height) + 0.1 *
                 job1->pos.x;
    float bot2 = job2->layer * GRID_HEIGHT * GRID_SIZE + (job2->pos.y + job2->source.height) + 0.1 *
                 job1->pos.x;

    if (bot1 > bot2)
    {
        return 1;
    }
    if (bot1 < bot2)
    {
        return -1;
    }
    return 0;
}

void DrawWorld()
{
    BeginMode2D(camera);

    // Draw Floor.
    {
        for (int y = 0; y < mapHeight; ++y)
        {
            for (int x = 0; x < mapWidth; ++x)
            {
                int sprite = spritesFloor[y * mapWidth + x];

                if (sprite == 0)
                {
                    continue;
                }

                sprite -= 1;

                int i = sprite % atlasWidth;
                int j = sprite / atlasWidth;

                Rectangle source = { i * spriteSize, j * spriteSize, spriteSize, spriteSize };
                Vector2 position = { x * spriteSize, y * spriteSize };
                DrawJob job = { source, position, WHITE, 0 };
                RegisterDraw(job);
            }
        }
    }

    // Draw Wall Sprites.
    {
        for (int y = 0; y < mapHeight; ++y)
        {
            for (int x = 0; x < mapWidth; ++x)
            {
                int sprite = wallSprites[y * mapWidth + x];

                if (sprite == 0)
                {
                    continue;
                }

                sprite -= 1;

                int i = sprite % atlasWidth;
                int j = sprite / atlasWidth;

                Rectangle source = { i * spriteSize, j * spriteSize, spriteSize, spriteSize };
                Vector2 position = { x * spriteSize, y * spriteSize };
                DrawJob job = { source, position, WHITE, 1 };
                RegisterDraw(job);
            }
        }
    }

    // Draw Decoration Sprites.
    {
        for (int y = 0; y < mapHeight; ++y)
        {
            for (int x = 0; x < mapWidth; ++x)
            {
                int sprite = spritesDecoration[y * mapWidth + x];

                if (sprite == 0)
                {
                    continue;
                }

                sprite -= 1;

                int i = sprite % atlasWidth;
                int j = sprite / atlasWidth;

                Rectangle source = { i * spriteSize, j * spriteSize, spriteSize, spriteSize };
                Vector2 position = { x * spriteSize, y * spriteSize };
                DrawJob job = { source, position, WHITE, 2 };
                RegisterDraw(job);
            }
        }
    }

    // Draw Tasks Sprites.
    {
        for (int y = 0; y < mapHeight; ++y)
        {
            for (int x = 0; x < mapWidth; ++x)
            {
                int sprite = spritesTasks[y * mapWidth + x];

                if (sprite == 0)
                {
                    continue;
                }

                sprite -= 1;

                int i = sprite % atlasWidth;
                int j = sprite / atlasWidth;

                Rectangle source = { i * spriteSize, j * spriteSize, spriteSize, spriteSize };
                Vector2 position = { x * spriteSize, y * spriteSize };
                DrawJob job = { source, position, WHITE, 2 };
                RegisterDraw(job);
            }
        }
    }

    // Draw Coworkers
    {
        for (int i = 0; i < nextCoworkerIdx; i++)
        {
            CoworkerRegisterDraw(coworkers + i);
        }
    }

    PlayerRegisterDraw(&player);

    // Draw Farts.
    {
        Color color = { 255, 255, 255, 200 };

        for (int y = 0; y < GRID_HEIGHT; ++y)
        {
            for (int x = 0; x < GRID_WIDTH; ++x)
            {
                switch (zone[y * GRID_WIDTH + x])
                {
                    case BIG:
                    {
                        Rectangle source = { 176, 112, spriteSize, spriteSize };
                        Vector2 position = { x * spriteSize, y * spriteSize };
                        DrawJob job = { source, position, color, 3 };
                        RegisterDraw(job);
                        break;
                    }
                    case SMALL:
                    {
                        Rectangle source = { 176 + 16, 112, spriteSize, spriteSize };
                        Vector2 position = { x * spriteSize, y * spriteSize };
                        DrawJob job = { source, position, color, 3 };
                        RegisterDraw(job);
                        break;
                    }
                    default:
                        break;
                }
            }
        }
    }

    // Sort draw jobs
    qsort(drawJobs, nextDrawJobIdx, sizeof(DrawJob), DrawComp);
    // Execute all draw jobs
    for (int i = 0; i < nextDrawJobIdx; ++i)
    {
        DrawJob job = drawJobs[i];
        DrawTextureRec(sprites, job.source, job.pos, job.tint);
    }
    // Clear draw jobs
    nextDrawJobIdx = 0;

    EndMode2D();

    // for (int y = 0; y < GRID_HEIGHT; ++y)
    // {
    //     for (int x = 0; x < GRID_WIDTH; ++x)
    //     {
    //         switch (map[y * GRID_WIDTH + x])
    //         {
    //             case WALL:
    //                 DrawRectangle(x * GRID_SIZE, y * GRID_SIZE, GRID_SIZE, GRID_SIZE, BLACK);
    //                 break;
    //             case DROPOFF:
    //                 DrawRectangle(x * GRID_SIZE, y * GRID_SIZE, GRID_SIZE, GRID_SIZE, RED);
    //                 break;
    //             case PICKUP:
    //                 DrawRectangle(x * GRID_SIZE, y * GRID_SIZE, GRID_SIZE, GRID_SIZE, ORANGE);
    //                 break;
    //             default:
    //                 break;
    //         }
    //     }
    // }
}

Cell GetCell(int x, int y)
{
    if (x >= GRID_WIDTH || x < 0 || y >= GRID_HEIGHT || y < 0)
    {
        return NONE;
    }

    return map[y * GRID_WIDTH + x];
}

// TODO(thismarvin): This is kinda... eh...
Cell GetTask(int x, int y)
{
    if (x >= GRID_WIDTH || x < 0 || y >= GRID_HEIGHT || y < 0)
    {
        return NONE;
    }

    return tasks[y * GRID_WIDTH + x];
}

Fart GetFart(Fart* targetZone, int x, int y)
{
    if (x >= GRID_WIDTH || x < 0 || y >= GRID_HEIGHT || y < 0)
    {
        return AIR;
    }

    return targetZone[y * GRID_WIDTH + x];
}

void SetFart(Fart* targetZone, int x, int y, Fart value)
{
    if (x >= GRID_WIDTH || x < 0 || y >= GRID_HEIGHT || y < 0)
    {
        return;
    }

    targetZone[y * GRID_WIDTH + x] = value;
}

Coord ScreenToGrid(float x, float y)
{
    int xGrid = (int)x / GRID_SIZE;
    int yGrid = (int)y / GRID_SIZE;

    Coord result = { xGrid, yGrid };
    return result;
}

Vector2 GridToScreen(int x, int y)
{
    float xScreen = x * GRID_SIZE + GRID_SIZE / 2;
    float yScreen = y * GRID_SIZE + GRID_SIZE / 2;

    Vector2 result = { xScreen, yScreen };
    return result;
}

int PickUpLookUp(int x, int y)
{
    return pickUpIds[y * GRID_WIDTH + x];
}

int GetPickUpCount(int x, int y)
{
    return pickUpCounts[y * GRID_WIDTH + x];
}

void DecrementPickUpCount(int x, int y)
{
    int tmp = pickUpCounts[y * GRID_WIDTH + x] - 1;

    tmp = MAX(0, tmp);

    // printf("%d\n", tmp);

    pickUpCounts[y * GRID_WIDTH + x] = tmp;
}

void IncrementTurns()
{
    turns += 1;
    turnNeedsToBeResolved = 1;
}

static void AddCoworker(Coord startPos, Coord endPos, int gender)
{
    Coworker* cw = coworkers + nextCoworkerIdx;
    int minMove = 1, maxMove = 3;
    int minWait = 2, maxWait = 4;
    float moveDuration = (maxMove - minMove) * (GetRandomValue(0, 1000) / 1000.0f) + minMove;
    float waitDuration = (maxWait - minWait) * (GetRandomValue(0, 1000) / 1000.0f) + minWait;
    CoworkerInit(cw, moveDuration, waitDuration, startPos, endPos, GRID_SIZE, gender);
    ++nextCoworkerIdx;
}
