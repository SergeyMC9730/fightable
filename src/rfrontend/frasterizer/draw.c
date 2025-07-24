
//          Sergei Baigerov 2024 - 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include "frfrontend/frasterizer.h"
#include <X11/Xlib.h>
#include <fraylib.h>
#ifdef TARGET_UNIX
#include <unistd.h>
#endif

#define TARGET_FPS 60
#define FRAME_DELAY (1000000 / TARGET_FPS)

void ClearBackground(Color color) {
#ifdef TARGET_SUPPORTS_X11
    XClearWindow(__frstate.display, __frstate.window);
#endif
}

void _fRasterizerTest() {
#ifdef TARGET_SUPPORTS_X11
    XDrawRectangle(__frstate.display, __frstate.window, __frstate.fb_context, 100, 100, 300, 300);
#endif
}

void BeginDrawing(void) {
#ifdef TARGET_UNIX
    gettimeofday(&__frstate.frame_begin_time, NULL);
#endif
}
void EndDrawing(void) {
#ifdef TARGET_SUPPORTS_X11
    XFlush(__frstate.display);
    __frstate.events_checked = 0;
    _fRasterizerProcessEvents();
#endif
#ifdef TARGET_UNIX
    struct timeval frame_end_time;
    gettimeofday(&frame_end_time, NULL);
    long long frame_time = (frame_end_time.tv_sec - __frstate.frame_begin_time.tv_sec) * 1000000L + (frame_end_time.tv_usec - __frstate.frame_begin_time.tv_usec);
    if (frame_time < (long long)FRAME_DELAY) {
        usleep(FRAME_DELAY - frame_time);
    }
#endif
}
