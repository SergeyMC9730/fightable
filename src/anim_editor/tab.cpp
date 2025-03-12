
//          Sergei Baigerov 2024 - 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include <fightable/anim_editor/tab.hpp>
#include <fightable/anim_editor/state.hpp>
#include <fightable/camera.h>
#include <fightable/sanitizer.h>
#include <fightable/color.h>
#include <math.h>

float fightable::tab::_MapEaseInterpolation(float x) {
    return x == 1.f ? 1.f : 1 - pow(2.f, -10.f * x);
}
bool fightable::tab::_CheckFloatAroundValue(float x, float r) {
    return (x > r) || (x < -r);
}

void fightable::tab::drawTabTitle() {
    Vector2 text_sz = MeasureTextEx(__state.unifont1, _title.c_str(), 16.f, 0.5f);
    float center_x = (_area.x / 2.f) + (_area.width - text_sz.x) / 2.f;

    Color shadow = BLACK; shadow.a = 128;
    RlDrawTextEx(__state.unifont1, _title.c_str(), {(_area.x / 2.f) + center_x + 1, _area.y + 2 + 1}, 16.f, 0.5f, shadow);
    RlDrawTextEx(__state.unifont1, _title.c_str(), {(_area.x / 2.f) + center_x, _area.y + 2}, 16.f, 0.5f, WHITE);
}
void fightable::tab::setCamPos(const Vector2 &v) {
    cam_logic.position = v;
}

fightable::tab::tab(RLRectangle area, Color col, const std::string &title) {
    _area = area;
    _col = col;
    _title = title;
    _cam = _fCameraLoadDefault();
    _cam.offset = {area.x, area.y};
}

void fightable::tab::visit() {
    if (cam_logic.movable) {
        float delta = GetFrameTime();

        Vector2 future_position = cam_logic.position;
        future_position.x += cam_logic._internal[1].x;
        future_position.y += cam_logic._internal[1].y;

        Vector2 v1;
        v1.x = future_position.x;
        v1.y = future_position.y;

        v1.x -= cam_logic._internal[1].x;
        v1.y -= cam_logic._internal[1].y;

        cam_logic._internal[0] = GetMousePosition();

        float accel_x = cam_logic._internal[1].x;
        float accel_y = cam_logic._internal[1].y;

        if (cam_logic.reverse_accel_x) {
            accel_x = -accel_x;
        }
        if (cam_logic.reverse_accel_y) {
            accel_y = -accel_y;
        }

        if (_horizontalMovement) cam_logic.position.x -= accel_x / _cam.zoom; // velocity x
        if (_verticalMovement) cam_logic.position.y -= accel_y / _cam.zoom;

        if (_limitScroll) {
            cam_logic.position.x = _fSanitizeFloat(cam_logic.position.x, _limitScrollSzMin.x, _limitScrollSzHigh.x);
            cam_logic.position.y = _fSanitizeFloat(cam_logic.position.y, _limitScrollSzMin.y, _limitScrollSzHigh.y);
        }

        bool is_negative_velocity_x = false;
        bool is_negative_velocity_y = false;

        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
            RLRectangle r1 = _area;

            if (CheckCollisionPointRec(cam_logic._internal[0], r1)) {
                cam_logic._internal[1] = GetMouseDelta();

                // cam_logic._internal[1].x *= delta * 100.f;
                // cam_logic._internal[1].y *= delta * 100.f;

                cam_logic._internal[3].x = cam_logic._internal[1].x < 0.f;
                cam_logic._internal[3].y = cam_logic._internal[1].y < 0.f;

                cam_logic._internal[2].x = 1.f;
                cam_logic._internal[2].y = 1.f;
            }
        } else {
            if ((_CheckFloatAroundValue(cam_logic._internal[1].x, 0.f) || _CheckFloatAroundValue(cam_logic._internal[1].y, 0.f)) && cam_logic._internal[2].y >= 1.f) {
                float interpolation = _MapEaseInterpolation(cam_logic._internal[2].x) * delta * 5.5f;

                float max_time = 30.f;

                cam_logic._internal[2].x -= delta / max_time;

                if (cam_logic._internal[3].x >= 1.f) {
                    if (cam_logic._internal[1].x <= 0.f) cam_logic._internal[1].x += interpolation;
                    else cam_logic._internal[1].x = 0.f;
                } else {
                    if (cam_logic._internal[1].x >= 0.f) cam_logic._internal[1].x -= interpolation;
                    else cam_logic._internal[1].x = 0.f;
                }
                if (cam_logic._internal[3].y >= 1.f) {
                    if (cam_logic._internal[1].y <= 0.f) cam_logic._internal[1].y += interpolation;
                    else cam_logic._internal[1].y = 0.f;
                } else {
                    if (cam_logic._internal[1].y >= 0.f) cam_logic._internal[1].y -= interpolation;
                    else cam_logic._internal[1].y = 0.f;
                }

                float old_velx = cam_logic._internal[1].x;
                float old_vely = cam_logic._internal[1].y;

                cam_logic._internal[1].x = fabs(cam_logic._internal[1].x);
                cam_logic._internal[1].y = fabs(cam_logic._internal[1].y);

                if (cam_logic._internal[1].x == interpolation) {
                    cam_logic._internal[1].x = 0.f;
                } else {
                    cam_logic._internal[1].x = old_velx;
                }

                if (cam_logic._internal[1].y == interpolation) {
                    cam_logic._internal[1].y = 0.f;
                } else {
                    cam_logic._internal[1].y = old_vely;
                }

                if ((cam_logic._internal[1].x == 0.f) && (cam_logic._internal[1].y == 0.f)) cam_logic._internal[2].x = 0.f; // stop moving

                // printf("vx %f vy %f time %f i %f | vx_neg %d vy_neg %d\n",
                //     cam_logic._internal[1].x, cam_logic._internal[1].y, cam_logic._internal[2].x, interpolation,
                //     (int)cam_logic._internal[3].x, (int)cam_logic._internal[3].y
                // );

                if (cam_logic._internal[2].x <= 0.f) cam_logic._internal[2].y = 0.f;
            } else {
                cam_logic._internal[1].x = 0.f;
                cam_logic._internal[1].y = 0.f;
            }
        }
    }

    _cam.target = cam_logic.position;

    BeginScissorMode(_area.x, _area.y, _area.width, _area.height);
    DrawRectangleRec(_area, _col);
    if (_limitScroll) {
        if ((_limitScrollSzHigh.y - _limitScrollSzMin.y) != 0.f) {
            float scroll_l = 30;
            float ratio = (_area.height - scroll_l) / (_limitScrollSzHigh.y - _limitScrollSzMin.y);
            float scroll_w = 8;

            DrawRectangleRec({_area.x + _area.width - scroll_w, _area.y + (_cam.target.y * ratio), scroll_w, scroll_l}, _fMixColors(_col, WHITE, 0.08f));

        }
        if ((_limitScrollSzHigh.x - _limitScrollSzMin.x) != 0.f) {
            float scroll_l = 40;
            float ratio = (_area.width - scroll_l) / (_limitScrollSzHigh.x - _limitScrollSzMin.x);
            float scroll_w = 8;

            DrawRectangleRec({_area.x + (_cam.target.x * ratio), _area.y, scroll_l, scroll_w}, _fMixColors(_col, WHITE, 0.08f));
        }
    }
    DrawRectangleLines(_area.x, _area.y, _area.width, _area.height, BLACK);
    drawTabTitle();
    BeginMode2D(_cam);
    draw();
    if (_limitScroll) {
        if ((_limitScrollSzHigh.y - _limitScrollSzMin.y) != 0.f) {
            DrawRectangleGradientV(_cam.target.x, fmax(_area.height, _limitScrollSzHigh.y + _area.height) - 100, _area.width, 100, BLANK, {0,0,0,127});
        }
        if ((_limitScrollSzHigh.x - _limitScrollSzMin.x) != 0.f) {
            DrawRectangleGradientH(fmax(_area.width, _limitScrollSzHigh.x + _area.width) - 100, 0, 100, fmax(_area.height, _limitScrollSzHigh.y + _area.height), BLANK, {0,0,0,127});
        }
    }
    EndMode2D();
    EndScissorMode();
}
