

#include <raylib.h>
#include <stdbool.h>

#include "collisionlayer.h"
#include "map.h"
#include "pixel_collider.h"
#include "player.h"

#ifdef PLATFORM_WEB
#include <emscripten/emscripten.h>
#endif

#define WINDOW_HEIGHT 900
#define WINDOW_WIDTH  1600

static Camera2D camera = {
    .target   = {0},
    .offset   = (Vector2){0, 5},
    .zoom     = 1,
    .rotation = 0,
};

static void HandleInput(void) {
    if (IsKeyDown(KEY_UP)) camera.target.y -= 1;
    if (IsKeyDown(KEY_DOWN)) camera.target.y += 1;

    if (camera.target.y < 0) camera.target.y = 0;
    if (camera.target.y > 800 - 225) camera.target.y = 800 - 225;
}

void GameLoop(void);

#define WIDTH  32
#define HEIGHT 32

static Texture texture;
static Texture map;
static Texture collisionLayer;
static Player player;
static RenderTexture2D renderTexture;

int main(int argc, char** argv) {
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Testing");

    const Image colLayerImg = COLLISIONLAYER_IMAGE;

    // map                  = LoadTextureFromImage(SIMPLE);
    collisionLayer  = LoadTextureFromImage(colLayerImg);

    player          = PlayerInit();
    renderTexture   = LoadRenderTexture(400, 225);
    camera.target.y = 800;

    PixelColliderInit(&colLayerImg);

#if PLATFORM_WEB
    emscripten_set_main_loop(GameLoop, 0, 1);
#else
    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        GameLoop();
    }
#endif

    UnloadTexture(texture);
    CloseWindow();
    return 0;
}

static bool isColliding = false;

void HandlePlayerCollision() {
    PlayerMove(&player);
    PlayerGravity(&player);
}

void GameLoop(void) {
    HandleInput();
    HandlePlayerCollision();

    BeginTextureMode(renderTexture);
    ClearBackground(PURPLE);

    BeginMode2D(camera);
    DrawTexture(collisionLayer, 0, 0, WHITE);
    PlayerDraw(&player);
    EndMode2D();
    EndTextureMode();

    Rectangle src = {0, 0, 400, -225};
    Rectangle dst = {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT};

    BeginDrawing();
    ClearBackground(RAYWHITE);
    DrawTexturePro(renderTexture.texture, src, dst, (Vector2){0}, 0, WHITE);
    EndDrawing();
}
