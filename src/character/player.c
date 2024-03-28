#include "player.h"

#include <math.h>
#include <stdbool.h>

#include "coin.h"
#include "pixel_collider.h"
#include "raylib.h"
#include "raymath.h"
#include "sprite.h"

Player PlayerInit() {
    return (Player){
        .speed           = 1.0,
        .sprite.texture  = LoadTextureFromImage(COIN_IMAGE),
        .sprite.cellY    = 3,
        .sprite.cellX    = 3,
        .sprite.position = (Vector2){25, 750},
        .sprite.cell     = 0,

        .collider.x      = 0,
        .collider.y      = 0,
        .collider.width  = 16,
        .collider.height = 16,
    };
}

void PlayerDraw(const Player* player) { SpriteDraw(&player->sprite); }

static bool PlayerSlope(Rectangle bounds) {
    Rectangle bottomRight = {
        .x      = bounds.x + bounds.width - 1,
        .y      = bounds.y + bounds.height - 1,
        .width  = 1,
        .height = 1,
    };

    Rectangle bottomLeft = {
        .x      = ceilf(bounds.x - 1),
        .y      = bounds.y + bounds.height - 1,
        .width  = 1,
        .height = 1,
    };

    Rectangle upperLeft  = bottomLeft;
    Rectangle upperRight = bottomRight;

    upperLeft.y -= 1.5;
    upperRight.y -= 1.5;

    bool rightB = PixelColliderOverlap(bottomRight);
    bool rightU = PixelColliderOverlap(upperRight);
    bool leftB  = PixelColliderOverlap(bottomLeft);
    bool leftU  = PixelColliderOverlap(upperLeft);

    bool result = (rightB && !rightU) || (leftB && !leftU);
    return result;
}

bool PlayerGrounded(Player* player) {
    Rectangle bounds = (Rectangle){
        .x      = player->sprite.position.x,
        .y      = player->sprite.position.y + player->collider.height,
        .width  = player->collider.width,
        .height = 1,
    };

    return PixelColliderOverlap(bounds);
}

void PlayerGravity(Player* player) {
    Rectangle bounds = PlayerBounds(player);
    bounds.y += player->gravity;
    player->gravity += 0.003;

    if (player->gravity > 1.0) player->gravity = 1.0;
    if (!PlayerGrounded(player)) {
        player->grounded          = false;
        player->sprite.position.y = bounds.y;
    } else {
        player->grounded          = true;
        player->gravity           = 0.1;
        player->sprite.position.y = (int)player->sprite.position.y;
    }
}

void PlayerMove(Player* player) {
    Vector2 movement = {0};

    if (IsKeyDown(KEY_W)) movement.y -= 1;
    if (IsKeyDown(KEY_S)) movement.y += 1;
    if (IsKeyDown(KEY_A)) movement.x -= 1;
    if (IsKeyDown(KEY_D)) movement.x += 1;
    if (movement.x == 0 && movement.y == 0) return;

    Vector2 normalized = Vector2Normalize(movement);
    movement.x         = normalized.x * player->speed;
    movement.y         = normalized.y * player->speed;

    float distance = sqrtf(movement.x * movement.x + movement.y * movement.y);
    int substeps   = (int)ceilf(distance);
    float substepMultiplier = 1.0 / substeps;

    Rectangle bounds        = PlayerBounds(player);
    bool horizontal         = false;
    bool vertical           = false;
    float slope             = 0.0;

    for (int i = 0; i < substeps; i++) {
        float old  = bounds.x;
        float dist = movement.x * substepMultiplier;
        bounds.x += dist;
        if (PlayerSlope(bounds)) slope -= 1.0;

        bounds.x = old;
        if (!vertical) {
            float old  = bounds.y;
            float dist = movement.y * substepMultiplier + slope;
            bounds.y += dist;

            if (PixelColliderOverlap(bounds)) {
                float dir = old - bounds.y;
                bounds.y  = (int)old;
                vertical  = true;
            }
        }

        if (!horizontal) {
            bounds.x += dist;

            if (PixelColliderOverlap(bounds)) {
                float dir  = old - bounds.x;
                bounds.x   = (int)old;
                horizontal = true;
            }
        }
    }

    player->sprite.position.x = bounds.x;
    player->sprite.position.y = bounds.y;
}

Rectangle PlayerBounds(const Player* player) {
    return (Rectangle){
        .x      = player->sprite.position.x + player->collider.x,
        .y      = player->sprite.position.y + player->collider.y,
        .width  = player->collider.width,
        .height = player->collider.height,
    };
}
