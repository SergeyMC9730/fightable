#include <fightable/editor.h>
#include <fightable/state.h>
#include <fightable/renderer.h>
#include <fightable/level.h>

void _fIntroMenuOnEditorCallback() {
    __state.current_editor = _fEditorCreate();

    __state.initial_game_size.x += __state.editor_size.x;
    __state.initial_game_size.y += __state.editor_size.y;

    __state.intro_can_continue = 1;

#ifndef TARGET_ANDROID
    if (!IsWindowFullscreen()) {
        SetWindowSize(__state.initial_game_size.x, __state.initial_game_size.y);
    }
#endif

    Vector2 dpi = GetWindowScaleDPI();

    UnloadRenderTexture(__state.framebuffer);
    UnloadRenderTexture(__state.overlay_framebuffer);

    __state.framebuffer = LoadRenderTexture(__state.initial_game_size.x / UI_SCALE / dpi.x, __state.initial_game_size.y / UI_SCALE / dpi.y);
    __state.overlay_framebuffer = LoadRenderTexture(__state.initial_game_size.x / dpi.x, __state.initial_game_size.y / dpi.y);
}

void _fIntroMenuOnEditor() {
    if (__state.menu_block_ui) return;

    _fGfxFadeOut(&__state.gfx, BLACK, 0.5);
    _fGfxSetFadeOutFunc(&__state.gfx, _fIntroMenuOnEditorCallback, 0);

    __state.menu_block_ui = 1;
}
