#pragma once

#include <fightable/intvec.h>
#include <fightable/level.h>
#include <fightable/block.h>
#include <fightable/entity.h>
#include <fightable/slider.h>

#include <map>
#include <vector>

struct feditor {
    std::vector<std::map<int, std::map<int, fblock>>> objects;
    std::vector<fblock> render_objects;

    fblock_listing block_listing;
    unsigned short current_block_id;

    Camera2D camera;
    struct flevel level;

    Texture2D select_block_label;

    unsigned char should_process_interactions : 1;
    unsigned char should_display_sidebar : 1;
    unsigned char should_playback : 1;
    unsigned char should_display_selector : 1;
    unsigned char f1_lock : 1;
    unsigned char swipe_enabled : 1;
    unsigned char holded_previosly : 1;
    unsigned char hp1 : 1;

    std::vector<fentity*> entities;

    unsigned short current_object_page;

    fslider test_slider;

    int current_layer;
};