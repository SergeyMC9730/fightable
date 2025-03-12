
//          Sergei Baigerov 2024 - 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include <cstdio>
#include <fightable/anim_editor/keyframe.hpp>
#include <fightable/anim_editor/viewpoint.hpp>
#include <fightable/anim_editor/keyframe_prop.hpp>
#include <fightable/anim_editor/state.hpp>

int main(int argc, char **argv) {
    InitWindow(1280, 720, "Animation Editor");

    SetTargetFPS(GetMonitorRefreshRate(0));

    if (argc > 1) {
        const char *file = argv[1];

        TraceLog(LOG_INFO, "Opening %s", file);

        __state.anim = _ntRendererLoadAnimation(file);
    }

    __state.unifont1 = LoadFont("assets/unifont-16.0.02.otf");

    std::vector<fightable::tab*> tabs = {
        new fightable::viewpoint_tab(),
        new fightable::keyframe_tab(),
        new fightable::keyframe_prop_tab()
    };

    while (!WindowShouldClose()) {
        BeginDrawing();

        for (auto a : tabs) {
            a->visit();
        }

        EndDrawing();
    }

    for (auto a : tabs) {
        delete a;
    }
    UnloadFont(__state.unifont1);

    RlCloseWindow();

    return 0;
}
