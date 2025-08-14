
//          Sergei Baigerov 2024 - 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

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

#define SETUP_BUTTON(NAME, Y, XO) SETUP_BUTTON_EX(NAME, Y, WHITE, XO)
#define SETUP_BUTTON_EX(NAME, Y, COLOR, XO) sz = _fButtonMeasureSizeSimple(NAME); if (_fButtonDrawSimple(NAME, (IVector2){(wx - sz) / 2 + __state.menu_cur_x + (float)(XO), Y}, COLOR))

void _fIntroMenuProcessMultiplayerJoin() {
    int wx = __state.framebuffer.texture.width;
    int wy = __state.framebuffer.texture.height;

    DrawTexture(__state.playbtn_container, 0, 0, WHITE);

    _fTextInputUpdate(__state.ip_input);
    _fTextInputRenderText(__state.ip_input);

    _fTextInputUpdate(__state.name_input);
    _fTextInputRenderText(__state.name_input);

    unsigned int sz = 0;

    SETUP_BUTTON("Join", 59, 45) {
        TraceLog(LOG_INFO, "Join");

        if (_fTextInputEmpty(__state.ip_input)) {
            _fNotifMgrSend("<cred,orange>ERROR:\n<cyellow>IP address is empty!");
        } else if (_fTextInputEmpty(__state.name_input)) {
            _fNotifMgrSend("<cred,orange>ERROR:\n<cyellow>Username field is empty!");
        } else {
            if (!__state.mp_client_connecting) {
                int id = _fNotifMgrSendWithTime("<cyellow>Connecting to the server...", 0);
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

                TraceLog(LOG_INFO, "Connecting to %s:%d (%d)", ip, (int)port, a->len);

                if (!_fMpClientConnect(ip, port)) {
                    _fAudioFxSlideVolume(&__state.sound_engine, 1, 1.f);
                    _fIntroGfxEnable();
                    _fTextInputUnlock(__state.ip_input);
                    _fTextInputUnlock(__state.name_input);
                    __state.mp_client_connecting = 0;
                }
            }
        }
    }

    SETUP_BUTTON("Exit", 78, 45) {
        TraceLog(LOG_INFO, "Exit");

        if (!__state.mp_client_connecting) {
            __state.menu_state = INTRO_MENU_BASE_SELECTOR;
            UnloadTexture(__state.playbtn_container);
        }
    }
}
