
//          Sergei Baigerov 2024 - 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <fightable/rendered_object.h>
#include <fightable/intvec.h>
#include <fightable/intrect.h>
#include <fightable/serializable.h>
#include <rsb/rsb_array_gen.h>
#include <tunnelos/unitype.h>

#include <pthread.h>

#define LEVEL_FORMAT_VERSION (uint16_t)4

#ifdef __cplusplus
extern "C" {
#endif

struct ftilemap;
struct fentity;
struct fblock;

RSB_ARRAY_DEF_GEN(struct fentity*, _fentity);

struct flevel_light_source {
    IVector2 pos;
    Color tint;
};

struct flevel_registry_entry {
    unsigned int id;
    unitype_t* entry;
};

struct flevel_chunk {
    RenderTexture2D obj;
    IVector2 chunk_offset;
};

RSB_ARRAY_DEF_GEN(struct flevel_light_source, _lls);
RSB_ARRAY_DEF_GEN(struct flevel_registry_entry, _lre);

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
    unsigned char in_gameover_mode : 1;

    rsb_array__fentity* entities;

    RLRectangle render_crop_area;

    float gameover_time;

    float cam_rot;

    rsb_array__lls* light_sources;
    rsb_array__lre* block_entries;

    unsigned int last_entry_id;

    pthread_t block_processor_thread;
    unsigned char block_p_close;
    unsigned char block_p_profile;

    unsigned char pause_world;
    float tps;

    IVector2 cam_offset;
};

void _fLevelDraw(struct flevel *level, IVector2 initial_pos);
void _fLevelDrawPixelated(struct flevel* level, IRectangle frame);
struct flevel *_fLevelLoadTest(struct ftilemap *tilemap, IVector2 background_tile);
RLRectangle *_fLevelGetHitboxes(struct flevel *level);
struct fentity *_fLevelFindPlayer(struct flevel *level);
fserializable _fLevelSerialize(struct flevel *level);
void _fLevelTriggerGameOver(struct flevel* level);
void _fLevelDestroyEntity(struct flevel* level, struct fentity* entity);
void _fLevelSave(struct flevel* level, const char* filename);
struct flevel* _fLevelLoadFromFile(const char* filename);
void _fLevelTick(struct flevel* level);
void *_fLevelDoBlockUpdate(void* level);
void _fLevelLoadProcessor(struct flevel *level);
void _fLevelDestroy(struct flevel* level, unsigned char level_allocated, unsigned char blocks_allocated, unsigned char entities_allocated);

void _fLevelLightSourceDraw(struct flevel* env, struct flevel_light_source* source);

#ifdef __cplusplus
}
#endif
