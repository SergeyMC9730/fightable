
//          Sergei Baigerov 2024 - 2026.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include "fightable/mp_shared.h"
#include "fightable/notif_mgr.h"
#include "fightable/text_input.h"
#include <fightable/intro.h>
#include <fightable/state.h>
#include <fightable/button.h>
#include <fightable/flags.h>
#include <fightable/slider.h>
#include <fightable/sound_engine.h>
#include <fightable/string.h>
#include <fightable/mp_client.h>
#include <fightable/translation.h>

#define SETUP_BUTTON(NAME, Y, XO, ALT) SETUP_BUTTON_EX(NAME, Y, WHITE, XO, ALT)
#define SETUP_BUTTON_EX(NAME, Y, COLOR, XO, ALT) sz = _fButtonMeasureSizeSimple(NAME); if (_fButtonDrawSimple(NAME, (IVector2){(wx - sz) / 2 + __state.menu_cur_x + (float)(XO), Y}, COLOR, ALT))

void _fIntroMenuProcessMultiplayerJoin() {
    int wx = __state.framebuffer.texture.width;
    int wy = __state.framebuffer.texture.height;

    DrawTexture(__state.playbtn_container, 0, 0, WHITE);

    // if (__state.mp_lobby_bg_ready) {
    //     float delta = GetFrameTime();
    //     __state.mp_lobby_bg_opacity += delta / 1.5f;
    //     if (__state.mp_lobby_bg_opacity > 1.f) {
    //         __state.mp_lobby_bg_opacity = 1.f;
    //     } else {
    //         TraceLog(LOG_INFO, "__state.mp_lobby_bg_opacity = %f;", __state.mp_lobby_bg_opacity);
    //     }


    //     _fMpDrawLobbyBackground(__state.mp_lobby_bg_opacity);
    // }

    _fTextInputUpdate(__state.ip_input);
    _fTextInputRenderText(__state.ip_input);

    _fTextInputUpdate(__state.name_input);
    _fTextInputRenderText(__state.name_input);

    unsigned int sz = 0;

    SETUP_BUTTON("Join", 59, 45, "menu.mp.join") {
        TraceLog(LOG_INFO, "Join");

        if (_fTextInputEmpty(__state.ip_input)) {
            _fNotifMgrSend(_fTranslationGetString("notification.mp.no_ip"));
        } else if (_fTextInputEmpty(__state.name_input)) {
            _fNotifMgrSend(_fTranslationGetString("notification.mp.no_username"));
        } else {
            if (!__state.mp_client_connecting) {
                int id = _fNotifMgrSendWithTime(_fTranslationGetString("notification.mp.connecting"), 0);
                __state.mp_client_notif_status = _fNotifMgrGetEntryById(id);

                _fTextInputLock(__state.ip_input);
                _fTextInputLock(__state.name_input);

                _fAudioFxSlideVolume(&__state.sound_engine, 0, 1.f);
                _fIntroGfxDisable();

                __state.mp_client_connecting = 1;

                rsb_array__pchar *a = _fSplitString((const char *)__state.ip_input->buffer, ':');

                const char *ip = "127.0.0.1";
                unsigned short port = 8000;

                if (a->len == 1) {
                    ip = a->objects[0];
                    port = 8000;
                } else if (a->len == 2) {
                    ip = a->objects[0];
                    port = (unsigned short)(abs(atoi(a->objects[1])) % 0xFFFF);
                }

                if (!_fMpClientConnect(ip, port)) {
                    _fAudioFxSlideVolume(&__state.sound_engine, 1, __state.config.volume_slider.progress);
                    _fIntroGfxEnable();
                    _fTextInputUnlock(__state.ip_input);
                    _fTextInputUnlock(__state.name_input);
                    __state.mp_client_connecting = 0;
                }

                _fCleanupSplittedString(a);
            }
        }
    }

    SETUP_BUTTON("Exit", 78, 45, "menu.mp.exit") {
        TraceLog(LOG_INFO, "Exit");

        if (!__state.mp_client_connecting) {
            __state.menu_state = INTRO_MENU_BASE_SELECTOR;
            UnloadTexture(__state.playbtn_container);
        }
    }
}
