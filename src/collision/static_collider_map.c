#include "static_collider_map.h"

StaticColliderMap StaticColliderMapInit(Rectangle bounds) {
    return (StaticColliderMap){
        .level         = MAX_LEVELS,
        .bounds        = bounds,
        .colliders     = {0},
        .nodes         = {0},
        .colliderCount = 0,
    };
}

void StaticColliderMapSplit(StaticColliderMap* map) {}

void StaticColliderMapIndex(StaticColliderMap* map, Rectangle bounds) {}

void StaticColliderMapInsert(StaticColliderMap* map, Collider* collider) {}

void StaticColliderMapRetrieve(StaticColliderMap* map,
                               Collider** overlaps,
                               Rectangle rect) {}
