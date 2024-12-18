#include <fightable/state.h>
#include <fightable/intro.h>
#include <fightable/renderer.h>
#include <fightable/rect.h>

void _fIntroMenuOnPlay() {
    if (__state.menu_block_ui) return;

    __state.menu_state = INTRO_MENU_PLAY;

    Texture2D singleplayer_label = _fTextRenderGradientV(&__state.text_manager, "Create", WHITE, GREEN, 1);
    Texture2D multiplayer_label = _fTextRenderGradientV(&__state.text_manager, "Join", WHITE, BLUE, 1);

    int wx = __state.framebuffer.texture.width;
    int wy = __state.framebuffer.texture.height;

    int w = 80;
    RLRectangle area = (RLRectangle){(wx - w) / 2, 45, w, 50};

    RenderTexture2D rt2d = LoadRenderTexture(area.width, area.height);
    BeginTextureModeStacked(rt2d);
    ClearBackground((Color){0, 0, 0, 64}); // (Color){0, 0, 0, 160}

    _fRectDraw((RLRectangle){0, 0, area.width - 1, area.height - 1}, WHITE, (Color){0x71, 0xaf, 0xfb, 0xff}, (Color){0, 0, 0, 0});

    int offset = 3;

    // BeginScissorMode(1, 1, area.width - 1, area.height - 1);

    // DrawRectanglePro((RLRectangle){-10 + area.x, 15 + area.y, 15, 15}, (Vector2){}, 45.f, GREEN);
    // DrawRectangle(0, offset, 15, 15, RED);
    // DrawRectanglePro((RLRectangle){3, 0 + offset, 15, 15}, (Vector2){}, 45.f, (Color){0, 0, 0, 16});
    // DrawRectanglePro((RLRectangle){1, 0 + offset, 15, 15}, (Vector2){}, 45.f, (Color){0, 0, 0, 16});
    // DrawRectanglePro((RLRectangle){0, 0 + offset, 15, 15}, (Vector2){}, 45.f, GREEN);
    DrawTexture(multiplayer_label, 27, 6 + offset, WHITE);
    _fTilemapDrawMegatile(__state.tilemap, (IVector2){5, 4}, (IVector2){29, 4}, (IVector2){2, 2}, 0, 0, WHITE);
    _fTilemapDrawMegatile(__state.tilemap, (IVector2){5, 4}, (IVector2){31, 4}, (IVector2){2, 2}, 0, 0, WHITE);

    offset -= 2;

    // DrawRectanglePro((RLRectangle){8, 26 + offset, 15, 15}, (Vector2){}, 65.f, RED);
    DrawTexture(singleplayer_label, 25, 33 + offset, WHITE);
    _fTilemapDrawMegatile(__state.tilemap, (IVector2){5, 29}, (IVector2){29, 4}, (IVector2){2, 2}, 0, 0, WHITE);
    _fTilemapDrawMegatile(__state.tilemap, (IVector2){5, 29}, (IVector2){33, 4}, (IVector2){2, 2}, 0, 0, WHITE);

    DrawLine(2, 23 + offset, area.width - 2, 23 + offset, (Color){0x71, 0xaf, 0xfb, 0xff});

    // EndScissorMode();

    EndTextureModeStacked();

    UnloadTexture(multiplayer_label);
    UnloadTexture(singleplayer_label);

    Image img = LoadImageFromTexture(rt2d.texture);
    ImageFlipVertical(&img);
    
    __state.playbtn_container = LoadTextureFromImage(img);
        
    UnloadImage(img);
    UnloadRenderTexture(rt2d);
}