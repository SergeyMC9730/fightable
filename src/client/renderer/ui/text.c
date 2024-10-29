#include <fightable/text.h>
#include <string.h>
#include <fightable/tilemap.h>
#include <stdio.h>
#include <fightable/renderer.h>

static const IVector2 __default_font_data[0xFF] = {
    {},{},{},{},{},{},{},{},{},{},{},{},{},{},{},{},{},{},{},{},{},{},{},{},{},{},{},{},{},{},{},{},
    {26,0}, // ' '
    {0,2}, {1,2}, {2,2}, {3,2}, {4,2}, {5,2}, {6,2}, {7,2}, {8,2}, {9,2}, {10,2}, {11,2}, {12,2}, {13,2}, {14,2},
    {0,1}, {1,1}, {2,1}, {3,1}, {4,1}, {5,1}, {6,1}, {7,1}, {8,1}, {9,1},
    {25,2}, {26,2}, {27,2}, {28,2}, {29,2}, {30,2}, {31,2},
    {0,0}, {1,0}, {2,0}, {3,0}, {4,0}, {5,0}, {6,0}, {7,0}, {8,0}, {9,0}, {10,0}, {11,0}, {12,0}, {13,0}, {14,0}, {15,0}, {16,0}, {17,0}, {18,0}, {19,0}, {20,0}, {21,0}, {22,0}, {23,0},{24,0},{25,0},
    {15,2}, {16,2}, {17,2}, {18,2}, {19,2}, {20,2},
    {0,0}, {1,0}, {2,0}, {3,0}, {4,0}, {5,0}, {6,0}, {7,0}, {8,0}, {9,0}, {10,0}, {11,0}, {12,0}, {13,0}, {14,0}, {15,0}, {16,0}, {17,0}, {18,0}, {19,0}, {20,0}, {21,0}, {22,0}, {23,0},{24,0},{25,0},
    {23,2}, {21,2}, {22,2}, {24,2}
};

IVector2 _fTextMeasure(struct ftext_manager *man, const char *text) {
    IVector2 result = {};

    if (!man) return result;

    int padding = man->char_padding;

    size_t len = strlen(text);
    for (size_t i = 0; i < len; i++) {
        char c = text[i];

        if (result.y == 0) {
            result.y = man->tilemap.tile_size.y;
        }

        if (c == '\n') {
            result.y += man->tilemap.tile_size.y + padding;

            continue;
        }

        result.x += man->tilemap.tile_size.x + padding;
    }

    if (result.x != 0) {
        result.x -= padding;
    }

    return result;
}

void _fTextDraw(struct ftext_manager *man, const char *text, IVector2 pos, Color color, unsigned char with_shadow) {
    if (!text || !man) return;

    IVector2 cur_pos = pos;
    size_t len = strlen(text);

    for (size_t i = 0; i < len; i++) {
        char c = text[i];
        IVector2 tile_pos = man->char_mapping[c];

        // printf("pos for '%c'(%d) is %d:%d\n", c, (int)c, tile_pos.x, tile_pos.y);

        if (c == '\n') {
            cur_pos.x = pos.x;
            cur_pos.y += man->tilemap.tile_size.y + man->char_padding;
        } else {
            if (with_shadow) {
                Color shadow = color;
                shadow.a /= 4.5;

                _fTilemapDraw(man->tilemap, (IVector2){cur_pos.x + 1, cur_pos.y + 1}, tile_pos, 0, 0, shadow);
            }

            _fTilemapDraw(man->tilemap, cur_pos, tile_pos, 0, 0, color);
            
            cur_pos.x += man->tilemap.tile_size.x + man->char_padding;
        }
    }
}

struct ftext_manager _fTextLoadDefault() {
    struct ftext_manager man = {};
 
    man.char_padding = 1;
    man.tilemap = _fTilemapCreate("text.png", (IVector2){3, 5});
    man.char_mapping = __default_font_data;

    return man;
}

#include <math.h>

unsigned char _hlpTool(unsigned char a, unsigned char b) {
    unsigned int _a = a;
    unsigned int _b = b;

    unsigned int mul = _a * _b;
    unsigned int res = fmin(0xFF, mul);

    return (unsigned char)res;
}

Texture2D _fTextRenderGradientV(struct ftext_manager *man, const char *text, Color top, Color bottom, unsigned char with_shadow) {
    if (!man || !text) return (Texture2D){};

    IVector2 sz = _fTextMeasure(man, text);
    sz.x++; sz.y++;

    RenderTexture2D output_char = LoadRenderTexture(man->tilemap.tile_size.x, man->tilemap.tile_size.y);
    RenderTexture2D output_char_grad = LoadRenderTexture(man->tilemap.tile_size.x, man->tilemap.tile_size.y);
    RenderTexture2D output = LoadRenderTexture(sz.x, sz.y);

    size_t len = strlen(text);
    IVector2 cur_pos = {};

    BeginTextureModeStacked(output_char_grad);
    DrawRectangleGradientV(0, 0, output_char_grad.texture.width, output_char_grad.texture.height, top, bottom);
    EndTextureModeStacked();

    Image gradient = LoadImageFromTexture(output_char_grad.texture);
    UnloadRenderTexture(output_char_grad);

    BeginTextureModeStacked(output);

    for (size_t i = 0; i < len; i++) {
        unsigned char c = text[i];

        if (c != ' ') {
            char cstr[2] = {c, 0};

            IVector2 cpos = man->char_mapping[c];

            BeginTextureModeStacked(output_char);
            ClearBackground(BLANK);
            _fTextDraw(man, cstr, (IVector2){0, 0}, WHITE, 0);
            EndTextureModeStacked();

            Image img = LoadImageFromTexture(output_char.texture);

            for (int x = 0; x < img.width; x++) {
                for (int y = 0; y < img.height; y++) {
                    Color c1 = GetImageColor(img, x, y);

                    if (c1.a != 0) {
                        ImageDrawPixel(&img, x, y, GetImageColor(gradient, x, y));
                    }
                }
            }

            Texture txt = LoadTextureFromImage(img);
            UnloadImage(img);

            BeginTextureModeStacked(output_char);
            ClearBackground(BLANK);

            DrawTexture(txt, 0, 0, WHITE);

            EndTextureModeStacked();
            UnloadTexture(txt);

            Rectangle source = (Rectangle){ 0, 0, (float)output_char.texture.width, (float)-output_char.texture.height };
            Rectangle dest = (Rectangle){ cur_pos.x, cur_pos.y, output_char.texture.width, output_char.texture.height };

            DrawTexturePro(output_char.texture, source, dest, (Vector2){0, 0}, 0.f, WHITE);
        }

        cur_pos.x += output_char.texture.width + man->char_padding;
        if (c == '\n') {
            cur_pos.x = 0;
            cur_pos.y += output_char.texture.height + man->char_padding;
        }
    }

    EndTextureModeStacked();

    UnloadImage(gradient);
    UnloadRenderTexture(output_char);

    Image output_image = LoadImageFromTexture(output.texture);
    Texture2D txt = LoadTextureFromImage(output_image);

    if (with_shadow == 0) {
        UnloadRenderTexture(output);
        UnloadImage(output_image);

        return txt;
    }

    RenderTexture2D helper_txt = LoadRenderTexture(output.texture.width, output.texture.height);
    BeginTextureModeStacked(helper_txt);
    ClearBackground(BLANK);

    Rectangle source = (Rectangle){ 0, 0, (float)txt.width, (float)-txt.height };
    Rectangle dest = (Rectangle){ 1, 0, txt.width, txt.height };
    DrawTexturePro(txt, source, dest, (Vector2){0, 0}, 0.f, (Color){255, 255, 255, 128});

    dest = (Rectangle){ 0, 1, txt.width, txt.height };
    DrawTexturePro(txt, source, dest, (Vector2){0, 0}, 0.f, WHITE);

    EndTextureModeStacked();

    UnloadImage(output_image);
    UnloadRenderTexture(output);
    UnloadTexture(txt);

    output_image = LoadImageFromTexture(helper_txt.texture);
    txt = LoadTextureFromImage(output_image);

    UnloadImage(output_image);
    UnloadRenderTexture(helper_txt);

    return txt;
}