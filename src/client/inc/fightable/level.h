#pragma once

#include <fightable/rendered_object.h>
#include <raylib.h>
#include <fightable/intvec.h>

#ifdef __cplusplus
extern "C" {
#endif

struct ftilemap;
struct fentity;
struct fblock;

struct flevel {
    struct fblock *objects;
    unsigned int data_size;
    unsigned short width;
    unsigned short height;

    Camera2D camera;
    IVector2 camera_size;

    struct ftilemap *tilemap;

    Texture2D background_tile;

    unsigned int objects_rendered;

    unsigned char in_workbench_mode : 1;

    struct fentity *entities;
    unsigned int entity_data_size;
};

void _fLevelDraw(struct flevel *level, IVector2 initial_pos);
struct flevel _fLevelLoadTest(struct ftilemap *tilemap, IVector2 background_tile);
Rectangle *_fLevelGetHitboxes(struct flevel *level);
struct fentity *_fLevelFindPlayer(struct flevel *level);

#ifdef __cplusplus
}
#endif
