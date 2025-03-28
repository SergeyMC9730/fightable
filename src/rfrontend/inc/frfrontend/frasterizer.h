
//          Sergei Baigerov 2024 - 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <frfrontend/rlapi.h>

#ifdef TARGET_SUPPORTS_X11
#include <X11/X.h>
#include <X11/Xlib.h>
#endif

enum frasterizer_key_state {
    KsIdle = 0,
    KsPressed, KsHeld, KsReleased
};

struct frasterizer_state {
#ifdef TARGET_SUPPORTS_X11
    Display *display;
    Window window;
    enum frasterizer_key_state *key_state;
    int pressed_key;
    unsigned char events_checked : 1;
#endif
    unsigned char ready : 1;
    unsigned char window_should_close : 1;
};

#ifdef __cplusplus
extern "C" {
#endif
extern struct frasterizer_state __frstate;

int _fRasterizerRaylibCharToNative(int rc);
int _fRasterizerNativeCharToIndex(int nc);

#ifdef __cplusplus
}
#endif
