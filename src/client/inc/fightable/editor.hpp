
//          Sergei Baigerov 2024 - 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <fightable/intvec.h>
#include <fightable/level.h>
#include <fightable/block.h>
#include <fightable/entity.h>
#include <fightable/slider.h>

#include <map>
#include <vector>

#include <PerlinNoise.hpp>

struct feditor;

template<unsigned int pages>
class feditor_panel {
protected:
    unsigned int _pages = pages;
    unsigned int _page = 0;

    Vector2 _position = {};

    virtual void setupPage(int page);
public:

    feditor_panel(feditor *instance);

    void setPage(int page);
    void draw();
};

struct feditor {
    std::vector<std::map<int, std::map<int, fblock>>> objects;
    std::vector<fblock> render_objects;

    fblock_listing block_listing;
    unsigned short current_block_id;

    Camera2D camera;
    struct flevel *level;

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

    fslider test_slider = {};

    int current_layer;

    bool in_edit_mode = false;
    RLRectangle edit_selection = {};
    std::vector<fblock> selected_objects = {};

    int button_page = 0;

    Model test_model = {};
    float test_z = 16;

    siv::PerlinNoise perlin;
};
