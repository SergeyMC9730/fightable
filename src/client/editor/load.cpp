#include <fightable/editor.hpp>
#include <fightable/editor.h>
#include <fightable/state.h>
#include <fightable/camera.h>
#include <fightable/renderer.h>
#include <fightable/storage.h>
#include <nfd.h>
#include <stdio.h>

struct feditor *_fEditorCreate() {
    feditor *editor = new feditor;
    editor->current_block_id = 0;
    editor->current_layer = -1;

    editor->camera = _fCameraLoadDefault();
    // editor->camera.target.x = 261340;

#ifndef TARGET_ANDROID
    nfdu8char_t* out_path;
    nfdu8filteritem_t filters[1] = { { "Level file", "bin" } };
    nfdopendialogu8args_t args = { 0 };
    args.filterList = nullptr;
    args.filterCount = 0;
    nfdresult_t result = NFD_OpenDialogU8_With(&out_path, &args);

    if (result != NFD_OKAY) {
        TraceLog(LOG_INFO, "Could not open file through file dialog (%d)", (int)result);
        editor->level = _fLevelLoadTest(__state.tilemap, { 28, 4 });
    }
    else {
        auto ref = _fLevelLoadFromFile(out_path);
        if (!ref) {
            TraceLog(LOG_INFO, "Could not open level");
            editor->level = _fLevelLoadTest(__state.tilemap, { 28, 4 });
        }
        else {
            TraceLog(LOG_INFO, "Opening chosen level");

            editor->level = *ref;
            MemFree(ref);
        }

        NFD_FreePathU8(out_path);
    }
#else
    editor->level = _fLevelLoadTest(__state.tilemap, { 28, 4 });
#endif
    editor->level.camera_size = {(int)((double)GetRenderWidth() / __state.window_scale), (int)((double)GetRenderHeight() / __state.window_scale)};

    editor->select_block_label = _fTextRenderGradientV(&__state.text_manager, "Select Block", WHITE, BLUE, 1);

    editor->render_objects.assign(editor->level.objects, editor->level.objects + editor->level.data_size);

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

    editor->test_model = LoadModel("test.obj");
    Camera3D test3d = {};
    test3d.fovy = 60;
    editor->test_cam = test3d;

    TraceLog(LOG_INFO, "Loaded %ld objects (%ld)", editor->render_objects.size(), editor->objects.size());
    TraceLog(LOG_INFO, "Viewable area: %d:%d virtual pixels", editor->level.camera_size.x, editor->level.camera_size.y);

    __state.sound_engine.do_not_shake = 1;

    return editor;
}