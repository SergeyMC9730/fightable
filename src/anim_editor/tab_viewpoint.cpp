
//          Sergei Baigerov 2024 - 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include "raylib.h"
#include <fightable/anim_editor/viewpoint.hpp>
#include <fightable/anim_editor/state.hpp>
#include <fightable/color.h>
#include <math.h>

void fightable::viewpoint_tab::DrawGrid2D(int sizeWidth, int sizeHeight, int spacing) {
    for (int i = -sizeHeight; i < sizeHeight; i += spacing) {
        DrawLine(-sizeWidth, i, sizeWidth, i, (Color){255, 255, 255, 15});
    }
    for (int i = -sizeWidth; i < sizeWidth; i += spacing) {
        DrawLine(i, -sizeHeight, i, sizeHeight, (Color){255, 255, 255, 15});
    }
}

fightable::viewpoint_tab::viewpoint_tab() : tab({0, 0, 1280, 512}, DARKGRAY, "Viewpoint") {
    _verticalMovement = true;
    _horizontalMovement = true;

    setCamPos({-_area.width / 2.f, -_area.height / 2.f});

    if (__state.anim == NULL) {
        _inDecart = true;
        return;
    }

    if (!_inDecart) {
        // SetRandomSeed(12);

        std::vector<int> entries;
        renderer_animation *child = __state.anim;
        while (child != NULL && entries.size() <= 256) {
            entries.push_back(child->anim_id);

            if (_pointColors.count(child->anim_id) == 0) {
                Color c = SKYBLUE;
                c.a = 64;
                // _pointColors[child->anim_id] = {(unsigned char)GetRandomValue(0, 255), (unsigned char)GetRandomValue(0, 255), (unsigned char)GetRandomValue(0, 255), 64};
                _pointColors[child->anim_id] = c;
            }

            child = child->linked_animation;
        }

        __state.anim->delta = 0.1f;
        float step = 0.1f;
        int seconds = 240;
        for (int i = 0; i < (int)((float)seconds / step); i++) {
            _ntRendererUpdateAnimation(__state.anim);

            for (int e : entries) {
                Vector2 v = {(float)i / (1.f / step), (float)_ntRendererGetAnimationResult(__state.anim, e)};
                _points[e].push_back(v);
                _maxHeight = fmax(_maxHeight, v.y);
            }
        }
    }
}

void fightable::viewpoint_tab::draw() {
    float wheel = GetMouseWheelMove();

    if (wheel != 0.f) {
        if (wheel < 0.f && _cam.zoom >= -0.5) {
            _cam.zoom += wheel / 8.f;
        }
        if (wheel > 0.f) {
            _cam.zoom += wheel / 8.f;
        }
    }

    if (IsKeyPressed(KEY_M)) {
        _inMacro = !_inMacro;
    }

    Color c = _fMixColors(_col, WHITE, 0.15f);
    DrawLine(-1000, 0, 1000, 0, c);
    DrawLine(0, -1000, 0, 1000, c);

    float radius = 5.f;

    // DrawCircle(0, 0, radius, WHITE);

    int limit = 100000 / 2;
    int limit_y = limit / 20;

    int scaling = 1.f;
    if (_inMacro && !_inDecart) {
        scaling = 8.f;
    }

    int spacing = 20;
    float text_size = 16.f;
    float text_spacing = 1.f;
    int line_length = 5;

    if (_cam.zoom < 1.f) {
        spacing = 40;
        text_size = 8.f;
    }
    if (_cam.zoom < 0.5f) {
        spacing = 70;
        text_size = 16.f;
    }
    if (_cam.zoom >= 3.f) {
        spacing = 10;
        text_size = 4.f;
    }
    if (_cam.zoom >= 9.f) {
        spacing = 5;
        text_size = 2.f;
        text_spacing = 0.5f;
        line_length = 2;
    }

    RLRectangle view_area = getVisibleCameraArea();

    char buf[64] = {0};

    DrawLine(-limit, 0, limit, 0, GRAY);

    for (int i = -limit; i < limit; i += spacing) {
        if (CheckCollisionPointRec((Vector2){(float)i, 0}, view_area)) {
            DrawLine(i, line_length, i, -line_length, GRAY);

            if (i != 0) {
                snprintf(buf, 64, "%d", (int)((float)i / (float)scaling));
                DrawTextPro(__state.unifont1, buf, (Vector2){(float)i, (float)line_length + 1}, (Vector2){}, 45.f, text_size, text_spacing, (Color){255, 255, 255, 64});
            }
        }
    }

    DrawLine(0, -limit_y, 0, limit_y, GRAY);

    for (int i = -limit_y; i < limit_y; i += spacing) {
        if (CheckCollisionPointRec((Vector2){0, (float)i}, view_area)) {
            DrawLine(-line_length, i, line_length, i, GRAY);

            if (i != 0) {
                snprintf(buf, 64, "%d", i / scaling);
                DrawTextPro(__state.unifont1, buf, (Vector2){(float)line_length + 1, (float)i}, (Vector2){}, 0.f, text_size, text_spacing, (Color){255, 255, 255, 64});
            }
        }
    }

    DrawGrid2D(limit, limit_y, 20);

    for (auto &[k, v] : _points) {
        std::vector<Vector2> new_strip = v;
        if (_inMacro) {
            for (auto &s : new_strip) {
                s.x *= (float)scaling;
                s.y *= (float)scaling;
            }
        }

        Color col = _pointColors[k];
        DrawLineStrip(new_strip.data(), new_strip.size(), col);
    }
}

RLRectangle fightable::viewpoint_tab::getVisibleCameraArea() {
    int offscreen = 25;

    RLRectangle r = {_cam.target.x - offscreen, _cam.target.y - offscreen};

    r.width = _area.width / _cam.zoom + (offscreen * 2);
    r.height = _area.height / _cam.zoom + (offscreen * 2);

    return r;
}
