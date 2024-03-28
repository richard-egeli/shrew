#include "pixel_collider.h"

#include <math.h>
#include <stdbool.h>
#include <stdint.h>

#include "raylib.h"

static PixelCollider map = {0};

void PixelColliderInit(const Image *image) {
    int width     = image->width;
    int height    = image->height;
    int length    = width * height;
    uint8_t *data = (uint8_t *)image->data;

    map.width     = image->width;
    map.height    = image->height;

    for (int i = 0, j = 0; j < length; i += 4, j++) {
        uint8_t r     = data[i];
        uint8_t g     = data[i + 1];
        uint8_t b     = data[i + 2];
        uint8_t a     = data[i + 3];

        map.pixels[j] = a != 0;
    }

    int countFalse = 0;
    int countTrue  = 0;

    for (int i = 0; i < length; i++) {
        if (map.pixels[i]) {
            countTrue++;
        } else {
            countFalse++;
        }
    }
}

bool PixelColliderOverlap(Rectangle bounds) {
    int startX = bounds.x;
    int startY = bounds.y;
    int endX   = bounds.x + bounds.width;
    int endY   = bounds.y + bounds.height;

    startX     = (startX < 0) ? 0 : startX;
    startY     = (startY < 0) ? 0 : startY;
    endX       = (endX > map.width) ? map.width : endX;
    endY       = (endY > map.height) ? map.height : endY;

    for (int y = startY; y < endY; y++) {
        for (int x = startX; x < endX; x++) {
            int index = (y * map.width) + x;
            if (index >= map.width * map.height) continue;
            if (map.pixels[index]) return true;
        }
    }

    return false;
}
