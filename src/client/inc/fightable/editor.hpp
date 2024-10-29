#pragma once

#include <map>
#include <fightable/intvec.h>
#include <raylib.h>
#include <fightable/level.h>
#include <vector>
#include <fightable/block.h>
#include <fightable/entity.h>

struct feditor {
    std::map<int, std::map<int, fblock>> objects;
    std::vector<fblock> render_objects;

    fblock_listing block_listing;
    unsigned short current_block_id;

    Camera2D camera;
    struct flevel level;

    unsigned char should_process_interactions : 1;
    unsigned char should_display_sidebar : 1;
    unsigned char should_playback : 1;

    std::vector<fentity> entities;

    bool f1_lock;
    bool swipe_enabled;

    bool holded_previosly;
    bool hp1;
};