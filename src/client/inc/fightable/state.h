#pragma once

#include <raylib.h>

#ifdef __cplusplus
extern "C" {
#endif

struct ftilemap;
struct flevel;
struct feditor;

#include <fightable/text.h>

struct fightable_state {
    struct ftilemap *tilemap;

    struct flevel *current_level;

    unsigned long frames_rendered;
    double time;

    RenderTexture2D framebuffer;

    struct feditor *current_editor;

    struct ftext_manager text_manager;

    unsigned long draw_calls;

    Music title_song;
};

extern struct fightable_state __state;

#ifdef __cplusplus
}
#endif
