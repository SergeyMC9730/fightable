
//          Sergei Baigerov 2024 - 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include "raylib.h"
#include <fraylib.h>
#include <fightable/state.h>
#include <fightable/renderer.h>
#include <nt5emul/tui/environment.h>

void _fLoaderMainProcessAssets(struct ftilemap *tilemapA, struct ftilemap *tilemapB) {
    TraceLog(LOG_INFO, "[LOADER] Loading game assets");

    *tilemapA = _fTilemapCreate("assets/textures/fightable1.png", (IVector2){8, 8});
    __state.tilemap = tilemapA;

    *tilemapB = _fTilemapCreate("assets/textures/fightable2.png", (IVector2){32, 32});
    __state.tilemap2 = tilemapB;

    __state.test_midground = LoadTexture("assets/textures/downsky_16bit_2.png");
    SetTextureWrap(__state.test_midground, TEXTURE_WRAP_REPEAT);

    int codepoint_amount = 0;
    int *codepoints = LoadCodepoints(_ntGetCodepoints(), &codepoint_amount);
    __state.unifont16 = LoadFontEx("assets/fonts/unifont-16.0.02.otf", 16, codepoints, codepoint_amount);
    __state.ubuntu_mono64 = LoadFontEx("assets/fonts/UbuntuMono-Regular.ttf", 64, codepoints, codepoint_amount);

    SetTextureFilter(__state.ubuntu_mono64.texture, TEXTURE_FILTER_TRILINEAR);

    __state.text_manager = _fTextLoadDefault();
}
