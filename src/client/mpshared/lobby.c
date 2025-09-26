
//          Sergei Baigerov 2024 - 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include "fightable/button.h"
#include "fightable/intvec.h"
#include "fightable/text.h"
#ifndef _DISABLE_MP_SERVER_

#include "fightable/renderer.h"
#include <fightable/mp_shared.h>
#include <fightable/state.h>
#include <fightable/sanitizer.h>
#include <fightable/rect.h>
#include <fightable/level.h>
#include <fightable/storage.h>
#include <fightable/mp_server.h>
#include <stdio.h>

void _fMpDrawLobby(float opacity, unsigned char interactable) {
    _fMpDrawLobbyBackground(opacity);

    unsigned char client = !__state.mp_server_ready;
    int port = (client) ? __state.mp_client_port : __state.mp_server_port;
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
    snprintf(buffer, sizeof(buffer) - 1, "Running on port\n %d\n\nJoin now!", port);

    _fTextDraw(&__state.text_manager, buffer, (IVector2) { safe_area_x, safe_area_y }, cblack, 0);

    const unsigned char simulate_player_rendering = 0;
    int clients = (simulate_player_rendering) ? MP_MAX_CLIENTS : __state.mp_connected_players->len;

    if (clients > 0) {
        int distance = (__state.framebuffer.texture.width - (safe_area_x * 2)) / clients;
        int list_y = __state.framebuffer.texture.height - safe_area_y - (ty * 2) - 5;

        unsigned char too_small = distance <= 20;
        if (too_small) {
            // distance *= 2;
            distance = (__state.framebuffer.texture.width - (safe_area_x * 2)) / (clients / 2);
        }

        for (int i = 0; i < clients; i++) {
            int offset = 0;
            int displayed_i = i;
            int h = clients / 2;

            if (too_small) {
                if (i < h) {
                    offset = -((ty * 2) + (__state.text_manager.tilemap.tile_size.y * 2) + 5);
                } else {
                    displayed_i -= h;
                    distance = (__state.framebuffer.texture.width - (safe_area_x * 2)) / (clients - (clients / 2));
                }
            } else {
                offset = -(__state.framebuffer.texture.height) / 3;
            }

            struct fplayer_connection *con =
                (!simulate_player_rendering)
                ? __state.mp_connected_players->objects + i
                : NULL
            ;

            unsigned char is_odd = ((displayed_i % 2) == 0);
            int x = safe_area_x + (distance * displayed_i) + (safe_area_x / 2);
            int y = list_y + offset;

            if (!too_small) {
                if (is_odd) {
                    y += ty / 2;
                } else {
                    y -= ty / 2;
                }
            }

            // TraceLog(LOG_INFO, "clients=%d; distance=%d; y=%d; x=%d; i=%d; di=%d", clients, distance, y, x, i, displayed_i);

            _fTilemapDrawScaled(__state.tilemap, (IVector2) { x + 2, y + 2 }, (IVector2) { 12, 0 }, 0, 0, cblack, 2.f);
            _fTilemapDrawScaled(__state.tilemap, (IVector2) { x, y }, (IVector2) { 12, 0 }, 0, 0, cwhite, 2.f);

            const char* nickname;

            if (!con) {
                snprintf(buffer, sizeof(buffer) - 1, "Player %d", i + 1);
                nickname = buffer;
            } else if (!con->username) {
                nickname = "Connecting";
            } else {
                if (con->is_local) {
                    snprintf(buffer, sizeof(buffer) - 1, "%s\n(you)", con->username);
                    nickname = buffer;
                } else if (con->is_owner) {
                    snprintf(buffer, sizeof(buffer) - 1, "%s\n(owner)", con->username);
                    nickname = buffer;
                } else {
                    nickname = con->username;
                }
            }

            IVector2 len = _fTextMeasure(&__state.text_manager, nickname);

            int center = ((tx * 2) - len.x) / 2;

            _fTextDraw(
                &__state.text_manager,
                nickname,
                (IVector2)
                    {
                        x + center,
                        !is_odd ?
                            y - __state.text_manager.tilemap.tile_size.y - 1
                            :
                            y + (ty * 2) + 1
                    },
                cwhite,
                1
            );
        }
    } else {
        const char *error = "No players\nconnected";
        IVector2 sz = _fTextMeasure(&__state.text_manager, error);
        IVector2 center = {
            (__state.framebuffer.texture.width - sz.x) / 2,
            (__state.framebuffer.texture.height - sz.y) / 2
        };
        _fTextDraw(&__state.text_manager, error, center, cwhite, 1);
    }

    const char *label = (client) ? (__state.mp_client_level) ? "current level" : "waiting for level" : "current level";

    IVector2 sel_sz = _fTextMeasure(&__state.text_manager, label);
    IVector2 text_pos = (IVector2){__state.framebuffer.texture.width - sel_sz.x - safe_area_x, safe_area_y};

    _fTextDraw(&__state.text_manager, label, text_pos, cyellow, 1);

    RLRectangle area = { text_pos.x, text_pos.y + sel_sz.y + 2, sel_sz.x, ty * 4 };

    Color container_color = cblack;
    container_color.a = (unsigned char)(64.f * opacity);
    _fRectDraw(area, cyellow, corange, container_color);

    struct flevel *level = (client) ? __state.mp_client_level : __state.current_level;

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

        _fLevelDrawPixelated(level, (IRectangle) { area.x + 1, area.y + 1, area.width - 1, area.height - 1 });
    }

    if (!client) {
        const char *button_label = "Open";
        int button_size = _fButtonMeasureSizeSimple(button_label);
        IVector2 button_placement = (IVector2) { area.x - button_size - 2, area.y };

        if (_fButtonDrawSimple(button_label, button_placement, cwhite) && interactable) {
            _fOpenFileSelector(_fStorageGetWritable(), _fMpOnOpenLevel);
        }

        if (__state.current_level) {
            button_label = "Play";
            button_size = _fButtonMeasureSizeSimple(button_label);
            button_placement.x = area.x - button_size - 2;
            button_placement.y += ty + 2;

            if (_fButtonDrawSimple(button_label, button_placement, cwhite) && interactable) {
                TraceLog(LOG_INFO, "TODO");
            }
        }
    } else {
        // if (!level) {
        //     _fTextDraw(&__state.text_manager, "Waiting for level", (IVector2){108, 17 + 15 + 2}, cblack, 0);
        // }
    }
}

#endif
