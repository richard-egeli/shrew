#ifndef INCLUDE_CWEB_COLLISION_STATIC_COLLIDER_MAP_H_
#define INCLUDE_CWEB_COLLISION_STATIC_COLLIDER_MAP_H_

#include <raylib.h>

#include "collider.h"

#define MAX_OBJECTS 10

#define MAX_LEVELS 5

typedef struct StaticColliderMap {
    int level;
    Rectangle bounds;
    Collider* colliders[MAX_OBJECTS];
    int colliderCount;
    struct StaticColliderMap* nodes[4];
} StaticColliderMap;

StaticColliderMap StaticColliderMapInit(Rectangle bounds);

void StaticColliderMapSplit(StaticColliderMap* map);

void StaticColliderMapIndex(StaticColliderMap* map, Rectangle bounds);

void StaticColliderMapInsert(StaticColliderMap* map, Collider* collider);

void StaticColliderMapRetrieve(StaticColliderMap* map,
                               Collider** overlaps,
                               Rectangle rect);

#endif  // INCLUDE_CWEB_COLLISION_STATIC_COLLIDER_MAP_H_
