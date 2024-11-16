#pragma once

#include <fightable/rendered_object.h>
#include <raylib.h>
#include <fightable/intvec.h>
#include <fightable/serializable.h>

#define LEVEL_FORMAT_VERSION (uint16_t)1

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

    struct fentity **entities;
    unsigned int entity_data_size;

#ifdef COTARGET_PTX
    Vector2 *dev_block_pipeline;
    Vector2 *host_block_pipeline;

    unsigned char *dev_allow_pipeline;
    unsigned char *host_allow_pipeline;

    int old_block_amount;
#endif
};

void _fLevelDraw(struct flevel *level, IVector2 initial_pos);
struct flevel _fLevelLoadTest(struct ftilemap *tilemap, IVector2 background_tile);
Rectangle *_fLevelGetHitboxes(struct flevel *level);
struct fentity *_fLevelFindPlayer(struct flevel *level);
fserializable _fLevelSerialize(struct flevel *level);
struct flevel _fLevelLoad(fserializable *serializable);

#ifdef COTARGET_PTX
void _fLevelReloadCudaCtx(struct flevel *level);
void _fLevelPrepareCudaRender(struct flevel *level, Rectangle area);
#endif

#ifdef __cplusplus
}
#endif
