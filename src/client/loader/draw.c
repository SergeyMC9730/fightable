
//          Sergei Baigerov 2024 - 2026.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include "raylib.h"
#include <fightable/state.h>
#include <fightable/renderer.h>
#include <fightable/storage.h>
#include <fightable/notif_mgr.h>
#include <fightable/translation.h>
#include <fightable/string.h>
#include <stdio.h>

char dbg_buffer[2048] = {};

unsigned char _fLoaderDraw() {
    Vector2 actual_sz = (Vector2){GetRenderWidth(), GetRenderHeight()};

    __state.base_game_size = actual_sz;

    _fAudioUpdateRaylibSounds(&__state.sound_engine);
    _fAudioFxUpdate(&__state.sound_engine);
    _fGfxUpdate(&__state.gfx);

    __state.gui_render_offset.x = __state.gfx.shake_v.x;
    __state.gui_render_offset.y = __state.gfx.shake_v.y;

    if (IsKeyPressed(KEY_G)) {
        // _fGfxShake(&__state.gfx, 4.f);
        _fGfxFadeInOut(&__state.gfx, BLACK, BLANK, 0.5f);
    }
    if (IsKeyPressed(KEY_F)) {
        _fOpenFileSelector(_fStorageGetWritable(), NULL);
    }
    if (IsKeyPressed(KEY_J)) {
        // const char *storage = _fStorageGetWritable();
        time_t tm = time(0);
        char *path = _fCopyString(TextFormat("scr_%ld.png", tm));

        TakeScreenshot(path);

        char *fmt = _fCopyString(TextFormat("%s%s", _fTranslationGetString("notification.editor.saved"), path));
        _fNotifMgrSend(fmt);

        free(path);
        free(fmt);
    }

    BeginTextureModeStacked(__state.framebuffer);

    _fDraw();
    _fAssistantClippyUpdate(&__state.clippy);
    _fAssistantClippyDraw(&__state.clippy);

    if (IsKeyPressed(KEY_F3)) {
        __state.show_debug_info = !__state.show_debug_info;
    }

    _fGfxDrawDamageOverlay();

    EndTextureModeStacked();

    BeginTextureModeStacked(__state.overlay_framebuffer);
    ClearBackground(BLANK);
    _fSchedulerIterateOverlays();
    _fTranslationRender();
    _fNotifMgrUpdate();
    EndTextureModeStacked();

    ClearBackground(BLACK);

    double scaling_y = (double)actual_sz.y / (double)__state.framebuffer.texture.height;
    int align_x = (actual_sz.x - (__state.framebuffer.texture.width * scaling_y)) / 2;

    __state.mouse_pos_offset = (Vector2){align_x, 0};
    __state.window_scale = scaling_y;

    {
        RLRectangle source = (RLRectangle){ 0, 0, (float)__state.framebuffer.texture.width, (float)-__state.framebuffer.texture.height };
        RLRectangle dest = (RLRectangle){ align_x, 0, (float)__state.framebuffer.texture.width * scaling_y, (float)__state.framebuffer.texture.height * scaling_y };

        DrawTexturePro(__state.framebuffer.texture, source, dest, (Vector2){0, 0}, 0.f, WHITE);
    }

    {
        double scaling_y = (double)actual_sz.y / (double)__state.overlay_framebuffer.texture.height;
        int align_x = (actual_sz.x - (__state.overlay_framebuffer.texture.width * scaling_y)) / 2;

        RLRectangle source = (RLRectangle){ 0, 0, (float)__state.overlay_framebuffer.texture.width, (float)-__state.overlay_framebuffer.texture.height };
        RLRectangle dest = (RLRectangle){ align_x, 0, (float)__state.overlay_framebuffer.texture.width * scaling_y, (float)__state.overlay_framebuffer.texture.height * scaling_y };

        DrawTexturePro(__state.overlay_framebuffer.texture, source, dest, (Vector2){0, 0}, 0.f, WHITE);
    }

    _fGfxDraw(&__state.gfx);

    if (__state.show_debug_info) {
        DrawFPS(32, 8);

        snprintf(dbg_buffer, 2048, "   offset: %d\n   ui scale: %f\n   window scale: %f\n   mus time: %f\n   playing: %s\n   song stage: %d\n   song id: %d\n   render area: %d:%d (%d:%d tiles)\n   gpu time: %fms\n   timer: %f\n   timer2: %f\n   shake data: %f %f",
            align_x,
            (float)UI_SCALE,
            (float)__state.window_scale,
            (float)_fAudioGetPlayTime(&__state.sound_engine),
            _fAudioGetSongName(&__state.sound_engine),
            __state.title_song_stage,
            (int)__state.song_id,
            __state.framebuffer.texture.width, __state.framebuffer.texture.height,
            __state.framebuffer.texture.width / __state.tilemap->tile_size.x, __state.framebuffer.texture.height / __state.tilemap->tile_size.y,
            __state.cuda_time,
            __state.damage_overlay_timer,
            __state.damage_overlay_timer2,
            __state.gui_render_offset.x,
            __state.gui_render_offset.y
        );

        RlDrawText(dbg_buffer, 8, 32, 20, RED);
    }

    _fSchedulerIteratePostDraws();

    __state.frames_rendered++;
    __state.time = GetTime();

    return !WindowShouldClose();
}
