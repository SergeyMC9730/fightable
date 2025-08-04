
//          Sergei Baigerov 2024 - 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include <fightable/intro.h>
#include <fraylib.h>
#include <fightable/state.h>
#include <stdio.h>
#include <fightable/button.h>
#include <stdlib.h>
#include <fightable/rect.h>
#include <fightable/renderer.h>
#include <fightable/level.h>
#include <fightable/editor.h>
#include <fightable/storage.h>

void _fIntroMenuOnMpJoin() {
    TraceLog(LOG_INFO, "Join menu");

    if (__state.menu_block_ui) return;

    __state.menu_state = INTRO_MENU_MULTIPLAYER_JOIN;

    UnloadTexture(__state.playbtn_container);

    RenderTexture2D rt2d = LoadRenderTexture(__state.framebuffer.texture.width, __state.framebuffer.texture.height);
    Texture2D ip_input = _fTextRenderGradientV(&__state.text_manager, "Enter server address", WHITE, (Color){0x91, 0xbf, 0xfb, 0xff}, 1);

    BeginTextureModeStacked(rt2d);

    ClearBackground((Color){0, 0, 0, 0}); // (Color){0, 0, 0, 160}
    DrawTexture(ip_input, 3, 10, WHITE);
    EndTextureModeStacked();

    UnloadTexture(ip_input);

    Image img = LoadImageFromTexture(rt2d.texture);
    ImageFlipVertical(&img);

    __state.playbtn_container = LoadTextureFromImage(img);

    UnloadImage(img);
    UnloadRenderTexture(rt2d);
}
