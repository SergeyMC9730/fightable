
//          Sergei Baigerov 2024 - 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include "nt5emul/renderer_animation.h"
#include "raylib.h"
#include <fightable/anim_editor/viewpoint.hpp>
#include <fightable/anim_editor/state.hpp>
#include <fightable/color.h>
#include <chrono>
#include <math.h>
#include <thread>

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
        std::map<int, int> curKeyframe;

        renderer_animation *child = __state.anim;

        while (child != NULL && entries.size() <= 256) {
            entries.push_back(child->anim_id);

            if (_pointColors.count(child->anim_id) == 0) {
                Color c = {(unsigned char)GetRandomValue(128, 255), (unsigned char)GetRandomValue(128, 255), (unsigned char)GetRandomValue(128, 255), 64};

                _pointColors[child->anim_id] = c;
                curKeyframe[child->anim_id] = 0;
                _keyframes[child->anim_id].push_back({0, (float)child->early_value});
            }

            child = child->linked_animation;
        }

        float step = 1.f / 10.f;
        int seconds = 60;

        __state.anim->delta = step;

        for (int i = 0; i < (int)((float)seconds / step); i++) {
            _ntRendererUpdateAnimation(__state.anim);
            for (int e : entries) {
                auto ep = _ntRendererGetEmbeddedAnimation(__state.anim, e);

                Vector2 v = {(float)i / (1.f / step), (float)ep->current_value};
                _points[e].push_back(v);

                // _maxHeight = fmax(_maxHeight, v.y);

                if (curKeyframe[e] != ep->current_keyframe) {
                    curKeyframe[e] = ep->current_keyframe;
                    _keyframes[e].push_back(v);
                }
            }
        }
    }

    _calcThread = std::thread([this](){
        while (!this->_stopCalcThread) {
           this->updatePointRendering();
           std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    });
}

void fightable::viewpoint_tab::draw() {
    float wheel = GetMouseWheelMove();

    if (wheel != 0.f) {
        float a = wheel / 4.f;
        _cam.zoom += a;
        if (_cam.zoom <= 0.f) _cam.zoom -= a;
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

    _limitScroll = true;
    _limitScrollSzMin = {(float)-limit, (float)-limit_y};
    _limitScrollSzHigh = {(float)limit, (float)limit_y};

    int scaling = getScaling();

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

    int drawn = 0;
    for (auto &[k, v] : _keyframes) {
        if (_drawablePoints.count(k)) {
            Color col = _pointColors[k];
            DrawLineStrip(_drawablePoints[k].data(), _drawablePoints[k].size(), col);
            drawn += _drawablePoints[k].size();
        }
    }

    _drawnPoints = drawn;
}

RLRectangle fightable::viewpoint_tab::getVisibleCameraArea() {
    int offscreen = 24;

    RLRectangle r = {_cam.target.x - offscreen, _cam.target.y - offscreen};

    r.width = _area.width / _cam.zoom + (offscreen * 2);
    r.height = _area.height / _cam.zoom + (offscreen * 2);

    return r;
}

void fightable::viewpoint_tab::postDraw() {
    int scaling = getScaling();

    auto m = getMousePos();

    for (auto &[k, v] : _keyframes) {
        Color col = _pointColors[k];
        col.a = 255;

        for (auto p : v) {
            if (_cam.zoom < 1.f) {
                DrawPixel(((p.x * (float)scaling) - _cam.target.x) * _cam.zoom, ((p.y * (float)scaling) - _cam.target.y) * _cam.zoom, col);
            } else {
                float radius = 4.f;
                RLRectangle r = {((p.x * (float)scaling) - _cam.target.x) * _cam.zoom - radius, ((p.y * (float)scaling) - _cam.target.y) * _cam.zoom - radius, radius * 2.f, radius * 2.f};
                RLRectangle tr = r;
                tr.x -= 8; tr.width += 16;
                tr.y -= 8; tr.height += 16;

                if (CheckCollisionPointRec(m, tr)) {
                    SetTextLineSpacing(16.f);
                    col = _fMixColors(col, WHITE, 0.2f);
                    DrawRectangleRec(r, col);

                    std::string info = "Time: " + std::to_string(p.x) + "\nValue: " + std::to_string(p.y) + "\nID: " + std::to_string(k);
                    Vector2 sz = MeasureTextEx(__state.unifont1, info.c_str(), 16.f, 0.5f);
                    float offset = 4.f;

                    DrawRectangle(r.x - offset + (m.x - r.x), r.y - offset + (m.y - r.y), sz.x + (offset * 2), sz.y + (offset * 2), {0,0,0,127});
                    RlDrawTextEx(__state.unifont1, info.c_str(), {r.x + (m.x - r.x), r.y + (m.y - r.y)}, 16.f, 0.5f, WHITE);
                } else {
                    DrawRectangleRec(r, col);
                }
            }
        }
    }

    _title = "Viewpoint (" + std::to_string(_drawnPoints) + ")";
}

int fightable::viewpoint_tab::getScaling() {
    int scaling = 1;
    if (_inMacro && !_inDecart) {
        scaling = 10;
    }

    return scaling;
}

void fightable::viewpoint_tab::updatePointRendering() {
    RLRectangle r = getVisibleCameraArea();
    int scaling = getScaling();

    this->_drawablePoints.clear();

    for (auto &[k, v] : _points) {
        std::vector<Vector2> new_strip;
        for (auto p : v) {
            if (_inMacro) {
                p.x *= (float)scaling;
                p.y *= (float)scaling;
            }
            if (p.x >= r.x && p.x <= r.x + r.width) {
                new_strip.push_back(p);
            }
        }

        _drawablePoints[k] = new_strip;
    }
}

fightable::viewpoint_tab::~viewpoint_tab() {
    TraceLog(LOG_INFO, "Closing viewpoint thread");

    this->_stopCalcThread = true;
    _calcThread.join();
}
