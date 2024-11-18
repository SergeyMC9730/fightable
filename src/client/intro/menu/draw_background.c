#include <fightable/intro.h>
#include <fightable/state.h>

void _fIntroMenuDrawBackground() {
    float delta = GetFrameTime();
    float speed = 24.f;
    __state.intro_bg_offsetf += delta * speed;

    int wx = __state.framebuffer.texture.width;
    int wy = __state.framebuffer.texture.height;

    Rectangle source = {0};
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
}