
//          Sergei Baigerov 2024 - 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
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
                printf("keypress\n");

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
                printf("keyrelease\n");

                KeySym ks = XLookupKeysym(&(e.xkey), 0);
                int idx = _fRasterizerNativeCharToIndex(ks);

                __frstate.key_state[idx] = KsReleased;

                break;
            }
        }
    }
#endif
}

void RlCloseWindow(void);

void InitWindow(unsigned int width, unsigned int height, const char *title) {
    {
#ifdef TARGET_SUPPORTS_X11
        const char *display_method = "X11";
        const char *runtime_device = "Desktop";
#else
        const char *display_method = "Unknown";
        const char *runtime_device = "Unknown";
#endif
        const char *render_method = "Software";

        printf("frasterizer: rendering in %s mode; displaying using %s on %s\n", render_method, display_method, runtime_device);
    }

    if (__frstate.ready) {
        printf("frasterizer: window already initialized\n");
        return;
    }

    if (width * height == 0) {
        printf("frasterizer: invalid window size\n");
        return;
    }
    if (title == NULL) {
        printf("frasterizer: invalid window title\n");
        return;
    }

#ifdef TARGET_SUPPORTS_X11
    const char *display_name = getenv("DISPLAY");
    __frstate.display = XOpenDisplay(display_name);

    if (__frstate.display == NULL) {
        printf("frasterizer: cannot open display\n");
        RlCloseWindow();
        return;
    }

    XSetWindowAttributes attributes = {};

    Visual *visual = NULL;
    int visual_depth = 0;

    {
        XVisualInfo visual_temp = {};
        int visual_amount = 0;
        XVisualInfo vinfo = {};

        visual_temp.screen = DefaultScreen(__frstate.display);

        XVisualInfo *visual_list = XGetVisualInfo(__frstate.display, VisualScreenMask, &visual_temp, &visual_amount);

        printf("frasterizer: Xlib provides these rendering modes:\n");
        for (int i = 0; i < visual_amount; i++) {
            printf("frasterizer:  %3d: visual 0x%lx class %d (%s) depth %d\n", i, visual_list[i].visualid, visual_list[i].class, visual_list[i].class == TrueColor ? "TrueColor" : "unknown", visual_list[i].depth);
        }

        printf("\nfrasterizer: trying to get access to 32-bit TrueColor mode\n");
        if (!XMatchVisualInfo(__frstate.display, XDefaultScreen(__frstate.display), 32, TrueColor, &vinfo)) {
            printf("frasterizer: mode does not exist\n");
            return;
        } else {
            printf("frasterizer: mode with id of %3ld can be used", vinfo.visualid);
        }

        XSync(__frstate.display, True);

        size_t fb_alloc = sizeof(Color) * width * height;
        __frstate.main_fb = (Color *)malloc(fb_alloc);
        memset(__frstate.main_fb, 0xFF, fb_alloc);

        __frstate.win_width = width;
        __frstate.win_height = height;

        attributes.colormap = XCreateColormap(__frstate.display, XDefaultRootWindow(__frstate.display), vinfo.visual, AllocNone);
        attributes.background_pixel = 0;
        attributes.border_pixel = 0;

        visual = vinfo.visual;
        visual_depth = vinfo.depth;
    }

    __frstate.key_state = (enum frasterizer_key_state *)malloc(sizeof(enum frasterizer_key_state) * __char_table_size);
    memset(__frstate.key_state, 0, sizeof(enum frasterizer_key_state) * __char_table_size);

    int screen = DefaultScreen(__frstate.display);
    __frstate.window = XCreateWindow(
        __frstate.display,
        XDefaultRootWindow(__frstate.display),
        100, 100,
        width, height,
        0,
        visual_depth,
        InputOutput,
        visual,
        CWBackPixel | CWColormap | CWBorderPixel,
        &attributes
    );

    __frstate.window_image = XCreateImage(
        __frstate.display,
        visual,
        32,
        XYPixmap,
        0,
        (char *)__frstate.main_fb,
        width,
        height,
        8,
        width * 4
    );

    if (!__frstate.window_image) {
        printf("frasterizer: cannot setup framebuffer\n");
        RlCloseWindow();
    }

    printf("frasterizer: window has been created\n");

    XGCValues values = {};
    __frstate.fb_context = XCreateGC(__frstate.display, __frstate.window, 0, &values);
    XSetForeground(__frstate.display, __frstate.fb_context, WhitePixel(__frstate.display, screen));
    XSetBackground(__frstate.display, __frstate.fb_context, BlackPixel(__frstate.display, screen));

    XSetLineAttributes(__frstate.display, __frstate.fb_context, 1, LineSolid, CapButt, JoinBevel);
    XSetFillStyle(__frstate.display, __frstate.fb_context, FillSolid);

    XSync(__frstate.display, 0);

    printf("frasterizer: created framebuffer context\n");

    XSelectInput(__frstate.display, __frstate.window, ExposureMask | KeyPressMask);
    XMapWindow(__frstate.display, __frstate.window);
    XFlush(__frstate.display);

    printf("frasterizer: mapped window to the display\n");
#else
    printf("frasterizer: frasterizer is not implemented for this platform\n");
    return;
#endif

    __frstate.ready = 1;
}

unsigned char WindowShouldClose(void) {
    _fRasterizerProcessEvents();

    return __frstate.window_should_close;
}

void RlCloseWindow(void) {
    if (__frstate.display) {
        XDestroyWindow(__frstate.display, __frstate.window);

        __frstate.display = NULL;
        memset(&__frstate.window, 0, sizeof(Window));
    }
    if (__frstate.main_fb) {
        free(__frstate.main_fb);

        __frstate.main_fb = NULL;
    }
}
