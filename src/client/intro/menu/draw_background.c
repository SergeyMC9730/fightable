#include "fightable/sound_library.h"
#include "raylib.h"
#include <fightable/intro.h>
#include <fightable/state.h>

void _fIntroMenuDrawBackground() {
    float delta = GetFrameTime();
    float speed = 24.f;
    __state.intro_bg_offsetf += delta * speed;

    int wx = __state.framebuffer.texture.width;
    int wy = __state.framebuffer.texture.height;

    RLRectangle source = {0};
    source.width = wx;
    source.height = wy;

    source.x -= ((long)__state.intro_bg_offsetf % __state.intro_temp_bg.width);
    source.y += ((long)__state.intro_bg_offsetf % __state.intro_temp_bg.height);

    RLRectangle dest = source;
    dest.x = 0;
    dest.y = 0;

    Color bg_color = (Color){0, 0, 200, 255};

    if (__state.song_id != MUS_CELESTIAL_FANTASIA) {
        DrawTexturePro(__state.intro_temp_bg, source, dest, (Vector2){}, 0.f, bg_color);
    } else {
        int offset_area = 16;
        DrawRectangle(-offset_area, 0, offset_area, wy, BLACK);
        DrawRectangle(wx, 0, offset_area, wy, BLACK);
        DrawRectangle(0, offset_area, -offset_area, wx, BLACK);
        DrawRectangle(0, wy, wx, offset_area, BLACK);
    }

    Color begin = (__state.song_id != MUS_CELESTIAL_FANTASIA) ? BLACK : BLUE;
    begin.a = 200;
    Color end = WHITE;
    end.a = 128;

    BeginBlendMode(BLEND_MULTIPLIED);

    DrawRectangleGradientV(0, 0, wx, wy, begin, end);

    EndBlendMode();

    // ClearBackground(BLUE);

    DrawTexture(__state.menu_borders, 0, 0, WHITE);
}
