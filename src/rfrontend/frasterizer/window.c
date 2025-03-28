
//          Sergei Baigerov 2024 - 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include "frfrontend/frasterizer.h"
#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <fraylib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern int *__char_table;
extern int __char_table_size;

void _fRasterizerProcessEvents() {
    if (__frstate.events_checked) return;
    __frstate.events_checked = 1;

#ifdef TARGET_SUPPORTS_X11
    XEvent e = {};

    while (XPending(__frstate.display) != 0) {
        XNextEvent(__frstate.display, &e);

        switch (e.type) {
            case KeyPress: {
                KeySym ks = XLookupKeysym(&(e.xkey), 0);
                int idx = _fRasterizerNativeCharToIndex(ks);

                if (__frstate.key_state[idx] == KsPressed) {
                    __frstate.key_state[idx] = KsHeld;
                } else {
                    __frstate.key_state[idx] = KsPressed;
                }

                __frstate.pressed_key = __char_table[idx];

                break;
            }
            case KeyRelease: {
                KeySym ks = XLookupKeysym(&(e.xkey), 0);
                int idx = _fRasterizerNativeCharToIndex(ks);

                __frstate.key_state[idx] = KsReleased;

                break;
            }
        }
    }
#endif
}

void InitWindow(unsigned int width, unsigned int height, const char *title) {
    if (__frstate.ready) {
        printf("window already initialized\n");
        return;
    }

#ifdef TARGET_SUPPORTS_X11
    __frstate.display = XOpenDisplay("123");

    if (__frstate.display == NULL) {
        printf("could not open display\n");
        return;
    }

    __frstate.key_state = (enum frasterizer_key_state *)malloc(sizeof(enum frasterizer_key_state) * __char_table_size);
    memset(__frstate.key_state, 0, sizeof(enum frasterizer_key_state) * __char_table_size);

    int screen = DefaultScreen(__frstate.display);
    __frstate.window = XCreateSimpleWindow(
        __frstate.display,
        RootWindow(__frstate.display, screen),
        0, 0,
        width, height,
        1,
        BlackPixel(__frstate.display, screen),
        WhitePixel(__frstate.display, screen)
    );

    XSelectInput(__frstate.display, __frstate.window, ExposureMask | KeyPressMask);
    XMapWindow(__frstate.display, __frstate.window);
#else
    printf("frasterizer is not implemented for this platform\n");
    return;
#endif

    __frstate.ready = 1;
}

unsigned char WindowShouldClose() {
    _fRasterizerProcessEvents();

    return __frstate.window_should_close;
}
