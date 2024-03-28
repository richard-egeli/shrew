#ifndef INCLUDE_CWEB_COLLISION_PIXEL_COLLIDER_H_
#define INCLUDE_CWEB_COLLISION_PIXEL_COLLIDER_H_

#include <raylib.h>
#include <stdbool.h>

#define TILE_SIZE            16
#define PIXEL_MAP_WIDTH      400
#define PIXEL_MAP_HEIGHT     800
#define COLLISION_ARRAY_SIZE (PIXEL_MAP_WIDTH * PIXEL_MAP_HEIGHT)

typedef struct PixelCollider {
    int width;
    int height;
    bool pixels[COLLISION_ARRAY_SIZE];
} PixelCollider;

void PixelColliderInit(const Image* image);

bool PixelColliderOverlap(Rectangle bounds);

#endif  // INCLUDE_CWEB_COLLISION_PIXEL_COLLIDER_H_
