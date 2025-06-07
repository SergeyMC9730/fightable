
//          Sergei Baigerov 2024 - 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include "PerlinNoise.hpp"
#include "fightable/level.h"
#include "fightable/notif_mgr.h"
#include "raylib.h"
#include <fightable/editor.hpp>
#include <fightable/editor.h>
#include <fightable/state.h>
#include <fightable/camera.h>
#include <fightable/renderer.h>
#include <fightable/storage.h>
#include <nfd.h>
#include <stdio.h>

void _fEditorOnFileSelected(struct nt_file_selector_menu *ctx, const char *path) {
    feditor *editor = __state.current_editor;

    if (!path) {
        TraceLog(LOG_INFO, "User aborted file selection");

        if (__state.current_level == NULL) {
            TraceLog(LOG_INFO, "Opening already loaded level");
            editor->level = __state.current_level;
        } else {
            TraceLog(LOG_INFO, "Opening template level");
            editor->level = _fLevelLoadTest(__state.tilemap, { 28, 4 });
        }
    } else {
        TraceLog(LOG_INFO, "Opening chosen level");
        auto ref = _fLevelLoadFromFile(path);
        if (!ref) {
            TraceLog(LOG_INFO, "Could not open level. Opening template level");
            _fNotifMgrSend("Could not open level");
            editor->level = _fLevelLoadTest(__state.tilemap, { 28, 4 });
        }
        else {
            TraceLog(LOG_INFO, "Opening chosen level");

            editor->level = ref;
        }
    }

    __state.current_level = editor->level;

    editor->level->camera_size = {(int)((double)GetRenderWidth() / __state.window_scale), (int)((double)GetRenderHeight() / __state.window_scale)};

    editor->select_block_label = _fTextRenderGradientV(&__state.text_manager, "Select Block", WHITE, BLUE, 1);

    editor->render_objects.assign(editor->level->objects, editor->level->objects + editor->level->data_size);

    for (const fblock &obj : editor->render_objects) {
        while (editor->objects.size() < (obj.layer_id + 1)) {
            editor->objects.push_back({});
        }

        editor->objects[obj.layer_id][obj.base.block_x][obj.base.block_y] = obj;
    }

    editor->block_listing = _fBlockGetAvailable();

    editor->should_display_sidebar = 1;
    editor->should_playback = 0;
    editor->should_process_interactions = 1;

    struct fslider slider = {0};
    slider.tint = BLUE;
    slider.scaling = UI_SCALE;
    slider.rect = RLRectangle{10, 10, 30, 4};
    slider.movable_width = 4.f;

    editor->test_slider = slider;

    editor->test_model = LoadModel(".fightable/test.obj");

    TraceLog(LOG_INFO, "Loaded %ld objects (%ld)", editor->render_objects.size(), editor->objects.size());
    TraceLog(LOG_INFO, "Viewable area: %d:%d virtual pixels", editor->level->camera_size.x, editor->level->camera_size.y);

    __state.sound_engine.do_not_shake = 1;

    if (editor->level->block_processor_thread == 0) {
        _fLevelLoadProcessor(editor->level);
    }

    __state.current_ui_menu = UI_MENU_EDITOR;

    _fCloseFileSelector();
}

struct feditor *_fEditorCreate() {
    feditor *editor = new feditor;

    editor->current_block_id = 0;
    editor->current_layer = -1;

    editor->camera = _fCameraLoadDefault();

    __state.current_editor = editor;
    // editor->camera.target.x = 261340;

    _fOpenFileSelector(_fStorageGetWritable(), _fEditorOnFileSelected);

    return editor;
}
