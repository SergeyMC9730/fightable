#include <fightable/state.h>
#include <fightable/intro.h>
#include <fightable/renderer.h>
#include <fightable/rect.h>

void _fIntroMenuOnOptions() {
    if (__state.menu_block_ui) return;

    __state.menu_state = INTRO_MENU_OPTIONS;

    int wx = 130;
    int wy = 52;

    IVector2 btncheck_pos = {10, 52};

    int w = 80;
    UnloadTexture(__state.playbtn_container);

    RenderTexture2D rt2d = LoadRenderTexture(wx, wy);

    Texture2D v_sync = _fTextRenderGradientV(&__state.text_manager, "V-Sync", WHITE, (Color){0x91, 0xbf, 0xfb, 0xff}, 1);
    Texture2D fullscreen = _fTextRenderGradientV(&__state.text_manager, "FullScreen", WHITE, (Color){0x91, 0xbf, 0xfb, 0xff}, 1);

    RLRectangle area = (RLRectangle){btncheck_pos.x + 10 + 2, btncheck_pos.y + 1, w, 10};

    BeginTextureModeStacked(rt2d);

    ClearBackground((Color){0, 0, 0, 0}); // (Color){0, 0, 0, 160}

    DrawTexture(v_sync, 3, 10, WHITE);
    DrawTexture(fullscreen, 3, 25, WHITE);

    EndTextureModeStacked();
    UnloadTexture(v_sync);
    UnloadTexture(fullscreen);

    Image img = LoadImageFromTexture(rt2d.texture);
    ImageFlipVertical(&img);
    
    __state.playbtn_container = LoadTextureFromImage(img);
        
    UnloadImage(img);
    UnloadRenderTexture(rt2d);

    _fConfigInit(&__state.config);
}