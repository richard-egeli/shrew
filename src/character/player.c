#include "player.h"

#include <math.h>
#include <stdbool.h>
#include <stdio.h>

#include "pixel_collider.h"
#include "raylib.h"
#include "shrew.h"
#include "sprite.h"

Player PlayerInit() {
    return (Player){
        .speed           = 0.5,
        .sprite.texture  = LoadTextureFromImage(SHREW_IMAGE),
        .sprite.cellY    = 1,
        .sprite.cellX    = 2,
        .sprite.position = (Vector2){25, 750},
        .sprite.cell     = 0,

        .collider.x      = 0,
        .collider.y      = 0,
        .collider.width  = 13,
        .collider.height = 8,

        .gravity         = 0.01,
        .gravityAccum    = 0.0,
    };
}

void PlayerDraw(Player* player) {
    static double timing = 0.5;
    static double accum;

    float time = GetTime();

    if (accum + timing < time) {
        int w = player->sprite.cellX;
        int h = player->sprite.cellY;
        int t = w * h;
        accum = time;

        player->sprite.cell += 1;
        player->sprite.cell = player->sprite.cell % t;
    }

    if (player->velocity.x > 0) {
        player->sprite.invertX = false;
    } else if (player->velocity.x < 0) {
        player->sprite.invertX = true;
    }

    SpriteDraw(&player->sprite);
}

static bool PlayerUpSlope(const Player* player, Rectangle bounds) {
    if (player->velocity.x > 0) {
        Rectangle bottomRight = {
            .x      = bounds.x + bounds.width - 1,
            .y      = bounds.y + bounds.height - 1,
            .width  = 1,
            .height = 1,
        };

        Rectangle upperRight = bottomRight;
        upperRight.y -= player->collider.height;
        upperRight.height = player->collider.height;

        bool rightB       = PixelColliderOverlap(bottomRight);
        bool rightU       = PixelColliderOverlap(upperRight);
        return rightB && !rightU;
    } else if (player->velocity.x < 0) {
        Rectangle bottomLeft = {
            .x      = bounds.x,
            .y      = bounds.y + bounds.height - 1,
            .width  = 1,
            .height = 1,
        };

        Rectangle upperLeft = bottomLeft;

        upperLeft.y -= player->collider.height;
        upperLeft.height = player->collider.height;

        bool leftB       = PixelColliderOverlap(bottomLeft);
        bool leftU       = PixelColliderOverlap(upperLeft);

        return leftB && !leftU;
    }

    return false;
}

static bool PlayerDownSlope(const Player* player, Rectangle bounds) {
    if (player->grounded) return false;

    if (player->velocity.x > 0) {
        Rectangle upperLeft = {
            .x      = ceilf(bounds.x - 1),
            .y      = bounds.y + bounds.height,
            .width  = 1,
            .height = 1,
        };

        Rectangle bottomLeft = upperLeft;

        bottomLeft.y += 1;

        bool leftU = PixelColliderOverlap(upperLeft);
        bool leftB = PixelColliderOverlap(bottomLeft);

        return !leftU && leftB;
    } else if (player->velocity.x < 0) {
        Rectangle upperRight = {
            .x      = bounds.x + bounds.width,
            .y      = bounds.y + bounds.height,
            .width  = 1,
            .height = 1,
        };

        Rectangle bottomRight = upperRight;
        bottomRight.y += 1;

        bool rightU = PixelColliderOverlap(upperRight);
        bool rightB = PixelColliderOverlap(bottomRight);
        return !rightU && rightB;
    }

    return false;
}

static bool PlayerGrounded(const Player* player) {
    if (player->jumping) return false;

    Rectangle bounds = (Rectangle){
        .x      = player->sprite.position.x,
        .y      = player->sprite.position.y + player->collider.height,
        .width  = player->collider.width,
        .height = 1,
    };

    return PixelColliderOverlap(bounds);
}

static bool PlayerCeiling(const Player* player) {
    Rectangle bounds = (Rectangle){
        .x      = player->sprite.position.x,
        .y      = player->sprite.position.y - 1,
        .width  = player->collider.width,
        .height = 1,
    };

    return PixelColliderOverlap(bounds);
}

void PlayerGravity(Player* player) {
    player->gravityAccum += player->gravity;

    if (player->gravityAccum > 1.0) player->gravityAccum = 1.0;

    if (!PlayerGrounded(player)) {
        player->grounded = false;
    } else {
        player->grounded          = true;
        player->gravityAccum      = 0.1;
        player->sprite.position.y = (int)player->sprite.position.y;
    }
}

void PlayerPhysicsStep(Player* player) {
    float x                 = player->velocity.x;
    float y                 = player->velocity.y;
    float distance          = sqrtf(x * x + y * y);
    int substeps            = (int)ceilf(distance);
    float substepMultiplier = 1.0 / substeps;

    Rectangle bounds        = PlayerBounds(player);
    bool horizontal         = false;
    bool vertical           = false;
    float slope             = 0.0;

    for (int i = 0; i < substeps; i++) {
        float old  = bounds.x;
        float dist = x * substepMultiplier;
        bounds.x += dist;

        if (!player->jumping) {
            if (PlayerUpSlope(player, bounds)) slope -= 1;
            if (PlayerDownSlope(player, bounds)) slope += 1;
        }

        bounds.x = old;
        if (!vertical) {
            float old  = bounds.y;
            float dist = y;

            dist *= substepMultiplier;
            dist += slope;

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

static void CalculateYVelocity(const Player* player) {
    float velocity = 0.0f;

    velocity += player->velocity.y;
}

bool PlayerJump(Player* player) {
    if (PlayerCeiling(player)) {
        player->ceiling = true;

        if (player->gravityAccum < 0) {
            player->gravityAccum = 0;
        }
        return false;
    } else {
        player->ceiling = false;
    }

    if (player->grounded && IsKeyDown(KEY_SPACE)) {
        player->jumping = true;
        return true;
    } else if (player->velocity.y >= 0) {
        player->jumping = false;
    }

    return false;
}

void PlayerMove(Player* player) {
    Vector2 movement = {0};

    if (IsKeyDown(KEY_A)) movement.x -= 1;
    if (IsKeyDown(KEY_D)) movement.x += 1;

    if (PlayerJump(player)) {
        player->gravityAccum = -1;
    }

    player->velocity.x = movement.x * player->speed;
    player->velocity.y = player->gravityAccum;
}

Rectangle PlayerBounds(const Player* player) {
    return (Rectangle){
        .x      = player->sprite.position.x + player->collider.x,
        .y      = player->sprite.position.y + player->collider.y,
        .width  = player->collider.width,
        .height = player->collider.height,
    };
}
