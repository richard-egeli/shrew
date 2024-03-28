#ifndef INCLUDE_CWEB_CHARACTER_PLAYER_H_
#define INCLUDE_CWEB_CHARACTER_PLAYER_H_

#include <raylib.h>

#include "sprite.h"

typedef struct Player {
    float speed;
    float gravity;
    bool grounded;
    Sprite sprite;
    Vector2 velocity;
    Rectangle collider;
} Player;

void PlayerGravity(Player* player);
void PlayerDraw(const Player* player);
void PlayerMove(Player* player);
Player PlayerInit(void);

Rectangle PlayerBounds(const Player* player);

#endif  // INCLUDE_CWEB_CHARACTER_PLAYER_H_
