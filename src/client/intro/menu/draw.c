#include <fightable/state.h>
#include <fightable/intro.h>

#ifdef TARGET_ANDROID
#include <android_native_app_glue.h>
#endif

void _fIntroMenuDraw() {
    _fIntroMenuDrawBackground();

    switch (__state.menu_state) {
        case INTRO_MENU_BASE_SELECTOR: {
            _fIntroMenuProcessBase();

<<<<<<< HEAD
    Rectangle source = {};
    source.width = wx;
    source.height = wy;

    source.x -= ((long)__state.intro_bg_offsetf % __state.intro_temp_bg.width);
    source.y += ((long)__state.intro_bg_offsetf % __state.intro_temp_bg.height);

    Rectangle dest = source;
    dest.x = 0;
    dest.y = 0;

    Color bg_color = (Color){0, 0, 200, 255};

    DrawTexturePro(__state.intro_temp_bg, source, dest, (Vector2){}, 0.f, bg_color);

    Color begin = BLACK;
    begin.a = 200;
    Color end = WHITE;
    end.a = 128;

    BeginBlendMode(BLEND_MULTIPLIED);

    DrawRectangleGradientV(0, 0, wx, wy, begin, end);

    EndBlendMode();

    // ClearBackground(BLUE);

    DrawTexture(__state.menu_borders, 0, 0, WHITE);
    if (__state.menu_pressed_play) {
        int w = 80;
        Rectangle area = (Rectangle){(wx - w) / 2, 45, w, 50};
        if (_fButtonDrawSimple("BACK", (IVector2){(wx - (3 * __state.tilemap->tile_size.x)) / 2, 95})) {
            __state.menu_pressed_play = 0;
            UnloadTexture(__state.playbtn_container);
        } else {
            DrawTexture(__state.playbtn_container, area.x, area.y, WHITE);
        }
    } else {
        if (_fButtonDrawSimple("PLAY", (IVector2){(wx - (3 * __state.tilemap->tile_size.x)) / 2, 50})) {
            __state.menu_pressed_play = 1;
            Texture2D singleplayer_label = _fTextRenderGradientV(&__state.text_manager, "Singleplayer", WHITE, (Color){0x91, 0xbf, 0xfb, 0xff}, 1);
            Texture2D multiplayer_label = _fTextRenderGradientV(&__state.text_manager, "Multiplayer", WHITE, (Color){0x91, 0xbf, 0xfb, 0xff}, 1);
=======
            break;
        }
>>>>>>> 244730706e8771db0dee66c77d76e5e1982f1ca8

        case INTRO_MENU_PLAY: {
            _fIntroMenuProcessPlay();

            break;
        }

        default: {}
    }
}