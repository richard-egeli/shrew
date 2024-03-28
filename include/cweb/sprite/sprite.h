#ifndef INCLUDE_CWEB_SPRITE_SPRITE_H_
#define INCLUDE_CWEB_SPRITE_SPRITE_H_

#include <raylib.h>

typedef struct Sprite {
    Vector2 position;
    Texture texture;
    int cellX;
    int cellY;
    int cell;
} Sprite;

void SpriteDraw(const Sprite* sprite);

#endif  // INCLUDE_CWEB_SPRITE_SPRITE_H_
