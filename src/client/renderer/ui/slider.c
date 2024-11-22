#include <fightable/slider.h>
#include <fightable/renderer.h>
#include <fightable/color.h>
#include <fightable/state.h>
#include <fightable/sanitizer.h>

void _fSliderDraw(struct fslider *slider) {
    if (!slider || slider->scaling <= 0.f) return;

    Rectangle movable_rect = {.x = slider->rect.x + (slider->rect.width * slider->progress), .y = slider->rect.y, .width = slider->movable_width * slider->scaling, .height = slider->rect.height * slider->scaling};

    Vector2 mouse = _fGetMousePosPix();
    mouse.x *= slider->scaling; mouse.y *= slider->scaling;

    Rectangle static_rect = slider->rect;
    static_rect.width *= slider->scaling; static_rect.height *= slider->scaling;

    FColor fc = _fColorToFloat(slider->tint);
    FColor fc1 = fc;
    FColor fc2 = fc;
    fc1.r *= 0.5f; fc1.g *= 0.5f; fc1.b *= 0.5f;
    fc2.r *= 0.15f; fc2.g *= 0.15f; fc2.b *= 0.15f;

    DrawRectangleRec(static_rect, _fFloatToColor(fc1));
    DrawRectangleLinesEx(static_rect, slider->scaling, _fFloatToColor(fc2));

    if (CheckCollisionPointRec(mouse, movable_rect)) {
        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
            float mdelta = GetMouseDelta().x;
            mdelta /= __state.window_scale / slider->scaling;

            movable_rect.x += mdelta;
            if (movable_rect.x > slider->rect.x + (slider->rect.width * slider->scaling)) {
                movable_rect.x -= mdelta;
                // TraceLog(LOG_INFO, "resetting 1");
            } else if (movable_rect.x < slider->rect.x) {
                TraceLog(LOG_INFO, "resetting 3");
                movable_rect.x = slider->rect.x;
            }

            fc.r *= 1.3f; fc.g *= 1.3f; fc.b *= 1.3f;
            _fFColorSanitize(&fc);

            slider->moving = 1;
        }
    } else if (slider->moving && CheckCollisionPointRec(mouse, static_rect)) {
        float mdelta = GetMouseDelta().x;
        mdelta /= __state.window_scale / slider->scaling;

        movable_rect.x += mdelta;
        if (movable_rect.x > slider->rect.x + ((slider->rect.width - slider->movable_width) * slider->scaling)) {
            movable_rect.x -= mdelta;
            // TraceLog(LOG_INFO, "resetting 2");
        } else if (movable_rect.x < slider->rect.x) {
            TraceLog(LOG_INFO, "resetting 4");
            movable_rect.x = slider->rect.x;
        }

        fc.r *= 1.3f; fc.g *= 1.3f; fc.b *= 1.3f;
        _fFColorSanitize(&fc);

        slider->moving = 1;
    }

    if (IsMouseButtonUp(MOUSE_BUTTON_LEFT)) {
        slider->moving = 0;
    }

    Rectangle movable_shadow_rect = movable_rect;
    movable_shadow_rect.x -= (slider->movable_width / 2.f) * slider->scaling;
    movable_shadow_rect.width += slider->movable_width * slider->scaling;

    slider->btn_relative_pos = (movable_rect.x - slider->rect.x) / slider->scaling;
    slider->progress = _fSanitizeFloat(slider->btn_relative_pos / slider->rect.width, 0.f, 1.f);

    DrawRectangleRec(movable_shadow_rect, (Color){0, 0, 0, 32});
    DrawRectangleRec(movable_rect, _fFloatToColor(fc));
}