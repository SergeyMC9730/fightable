
//          Sergei Baigerov 2024 - 2026.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include "fightable/intvec.h"
#include <fightable/singleplayer.h>
#include <fraylib.h>
#include <fightable/storage.h>
#include <fightable/sound_engine.h>
#include <fightable/state.h>
#include <fightable/mp_shared.h>
#include <fightable/sound_library.h>
#include <fightable/rect.h>
#include <fightable/renderer.h>
#include <fightable/level.h>
#include <fightable/editor.h>
#include <fightable/block_library.h>
#include <fightable/player.h>

void _fSingleplayerJoinWorld() {
    struct flevel *level = __state.current_level;
    _fLevelPerformBasicSpawn(level, 1);
    _fLevelLoadProcessor(__state.current_level);
    __state.sp_opened_level = 1;
}

void _fSingleplayerOnOpenLevel(struct nt_file_selector_menu *ctx, const char *path) {
    struct flevel *lvl = _fLevelLoadFromFileSelector(path);
    lvl->pause_world = 1;

    if (lvl != __state.current_level && __state.current_level) {
        unsigned char src = __state.current_level->level_source;
        _fLevelDestroy(__state.current_level, 1, (src != LEVEL_SOURCE_EDITOR), (src != LEVEL_SOURCE_EDITOR));
        __state.current_level = NULL;
    }

    __state.current_level = lvl;

    _fCloseFileSelector();
}

void _fSingleplayerDrawLobby() {
    if (__state.sp_opened_level) {
        _fLevelDraw(__state.current_level, (IVector2){});
        return;
    }
    float opacity = 1.f;

    _fMpDrawLobbyBackground(opacity);

    Color cwhite = WHITE;
    Color cblack = BLACK;
    Color cyellow = YELLOW;
    Color corange = ORANGE;

    cwhite.a = (unsigned char)(255.f * opacity);
    cblack.a = cwhite.a;
    cyellow.a = cblack.a;
    corange.a = cyellow.a;

    int tx = __state.tilemap->tile_size.x;
    int ty = __state.tilemap->tile_size.y;

    int safe_area_x = tx;
    int safe_area_y = ty;

    char buffer[48] = {};

    Vector2 vsz = _fGetCurrentFramebufferSize();
    // snprintf(buffer, sizeof(buffer) - 1, "Running on port\n %d\n\nJoin now!", port);

    // _fTextDraw(&__state.text_manager, buffer, (IVector2) { safe_area_x, safe_area_y }, cblack, 0);

    const char *label = (__state.current_level) ? "current level" : "waiting for level";
    const char *label2 = "select world";
    IVector2 sel_sz = _fTextMeasure(&__state.text_manager, label);
    IVector2 swr_sz = _fTextMeasure(&__state.text_manager, label2);

    _fTextDraw(&__state.text_manager, label2, (IVector2){(vsz.x - swr_sz.x) / 2, 5}, cblack, 0);

    IVector2 area_sz = {vsz.x * 0.65, 45};
    RLRectangle area = { (vsz.x - area_sz.x) / 2, (vsz.y - area_sz.y) / 2, area_sz.x, area_sz.y };

    _fTextDraw(&__state.text_manager, label, (IVector2){(vsz.x - sel_sz.x) / 2, area.y - sel_sz.y - 5}, cyellow, 1);

    Color container_color = cblack;
    container_color.a = (unsigned char)(64.f * opacity);
    _fRectDraw(area, cyellow, corange, container_color);

    struct flevel *level = __state.current_level;

    struct fLDPResult r = {};
    if (level != NULL) {
        Vector2 mouse = _fGetMousePosPix();

        if (CheckCollisionPointRec(mouse, area) && IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
            __state.mp_lobby_lp_moving = 1;
        }

        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT) && __state.mp_lobby_lp_moving) {
            Vector2 d = GetMouseDelta();
            level->camera.target.x -= d.x / UI_SCALE;
            level->camera.target.y -= d.y / UI_SCALE;
        }

        if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
            __state.mp_lobby_lp_moving = 0;
        }

        r = _fLevelDrawPixelated(level, (IRectangle) { area.x + 1, area.y + 1, area.width - 1, area.height - 1 });
    }

    const char *button_label = "Open";
    int button_size = _fButtonMeasureSizeSimple(button_label);
    IVector2 button_placement = (IVector2) { area.x, area.y + area.height + 2};

    if (_fButtonDrawSimple(button_label, button_placement, cwhite, "mp.lobby.open")) {
        _fOpenFileSelector(_fStorageGetWritable(), _fSingleplayerOnOpenLevel);
    }

    if (__state.current_level && r.has_startpos) {
        button_label = "Play";
        button_size = _fButtonMeasureSizeSimple(button_label);
        button_placement.x = area.x + area.width - button_size;

        if (_fButtonDrawSimple(button_label, button_placement, cwhite, "mp.lobby.join")) {
            _fSingleplayerJoinWorld();
        }
    }
}

void _fSingleplayerJoinLobbyCallback(void *ctx) {
    float* old_vol = (float*)ctx;
    if (!old_vol) return;

    char* buffer = (char*)MemAlloc(256);
    const char* readable = _fStorageGetWritable();

    _fAudioStop(&__state.sound_engine);

    _fMpCreateLobbyBackground();

    snprintf(buffer, 256, "%s/assets/music/3g_crim.xm", readable);

    _fAudioPlayModule(&__state.sound_engine, buffer);
    _fAudioLoopCurrent(&__state.sound_engine);
    _fAudioFxSlideVolume(&__state.sound_engine, *old_vol, 0.5f);

    MemFree(old_vol);
    MemFree(buffer);

    __state.song_id = MUS_3G_ANTS;
    __state.current_ui_menu = UI_MENU_SPJOIN;
    __state.mp_create_time = 0;
}

void _fSingleplayerJoinLobby() {
    float* old_vol = (float *)MemAlloc(sizeof(float));
    *old_vol = _fAudioGetVolume(&__state.sound_engine);

    _fGfxFadeOut(&__state.gfx, BLACK, 0.5);
    _fGfxSetFadeOutFunc(&__state.gfx, _fSingleplayerJoinLobbyCallback, old_vol);

    _fAudioFxSlideVolume(&__state.sound_engine, 0.f, 0.4f);
}
