#include <fightable/editor.hpp>
#include <fightable/editor.h>
#include <fightable/state.h>
#include <fightable/camera.h>
#include <stdio.h>

struct feditor *_fEditorCreate() {
    feditor *editor = new feditor;

    editor->camera = _fCameraLoadDefault();
    // editor->camera.target.x = 261340;

    editor->level = _fLevelLoadTest(__state.tilemap, {28, 4});
    editor->level.camera_size = {GetRenderWidth() / 5, GetRenderHeight() / 5};

    editor->render_objects.assign(editor->level.objects, editor->level.objects + editor->level.data_size);

    for (fblock &obj : editor->render_objects) {
        editor->objects[obj.base.block_x][obj.base.block_y] = obj;
    }

    editor->block_listing = _fBlockGetAvailable();

    editor->should_display_sidebar = 1;
    editor->should_playback = 0;
    editor->should_process_interactions = 1;

    printf("loaded %ld objects (%ld)\n", editor->render_objects.size(), editor->objects.size());    

    return editor;
}