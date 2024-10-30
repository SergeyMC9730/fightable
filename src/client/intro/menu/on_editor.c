#include <fightable/editor.h>
#include <fightable/state.h>
#include <fightable/renderer.h>
#include <fightable/level.h>

void _fIntroMenuOnEditor() {
    __state.current_editor = _fEditorCreate();
    MemFree(__state.current_level->objects);
    __state.current_level->objects = NULL;

    __state.current_level = NULL;

    __state.initial_game_size.x += __state.editor_size.x;
    __state.initial_game_size.y += __state.editor_size.y;

    __state.intro_can_continue = 1;

#ifndef TARGET_ANDROID
    SetWindowSize(__state.initial_game_size.x, __state.initial_game_size.y);
#endif

    UnloadRenderTexture(__state.framebuffer);
    __state.framebuffer = LoadRenderTexture(__state.initial_game_size.x / UI_SCALE, __state.initial_game_size.y / UI_SCALE);
}