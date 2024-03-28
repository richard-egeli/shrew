#include "sprite.h"

#include <math.h>
#include <raylib.h>

void SpriteDraw(const Sprite* sprite) {
    int width      = sprite->texture.width;
    int height     = sprite->texture.height;
    int cellX      = sprite->cellX;
    int cellY      = sprite->cellY;
    int cell       = sprite->cell;

    float cellW    = (float)width / cellX;
    float cellH    = (float)height / cellY;
    int cellPosX   = cell % cellX;
    int cellPosY   = cell / cellX;
    Rectangle rect = {
        .x      = cellPosX * cellW,
        .y      = cellPosY * cellH,
        .width  = cellW,
        .height = cellH,
    };

    Vector2 position = {
        .x = (int)sprite->position.x,
        .y = (int)sprite->position.y,
    };

    DrawTextureRec(sprite->texture, rect, position, WHITE);
}
