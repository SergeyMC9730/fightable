
//          Sergei Baigerov 2024 - 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include "fightable/level.h"
#include <fightable/editor.hpp>
#include <fightable/editor.h>
#include <fightable/state.h>
#include <fightable/camera.h>
#include <fightable/renderer.h>
#include <fightable/storage.h>
#include <fightable/pthread_compat.h>
#include <stdio.h>

void _fEditorOnFileSelected(struct nt_file_selector_menu *ctx, const char *path) {
    feditor *editor = __state.current_editor;

    struct flevel *lvl = _fLevelLoadFromFileSelector(path);

    if (lvl != __state.current_level && __state.current_level) {
        unsigned char src = __state.current_level->level_source;
        _fLevelDestroy(__state.current_level, 1, (src != LEVEL_SOURCE_EDITOR), (src != LEVEL_SOURCE_EDITOR));
        __state.current_level = NULL;
    }

    editor->level = lvl;

    __state.current_level = editor->level;
    __state.current_level->level_source = LEVEL_SOURCE_EDITOR;

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

    if (_fComparePthreadAndEmptyThread(editor->level->block_processor_thread)) {
        TraceLog(LOG_WARNING, "Tick thread was not allocated for this level");
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
