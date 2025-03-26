
//          Sergei Baigerov 2024 - 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include "frfrontend/frasterizer.h"
#include <X11/X.h>
#include <X11/Xlib.h>
#include <fraylib.h>
#include <stdio.h>

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

#endif

    __frstate.ready = 1;
}

unsigned char WindowShouldClose() {
    if (!__frstate.events_checked) {
        XEvent e;
        XNextEvent(__frstate.display, &e);

        if (e.type == KeyPress) {

        }
    }

    return __frstate.window_should_close;
}
