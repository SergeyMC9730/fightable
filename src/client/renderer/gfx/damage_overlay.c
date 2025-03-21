
//          Sergei Baigerov 2024 - 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include <fightable/state.h>
#include <fightable/sanitizer.h>
#include <fightable/renderer.h>

void _fGfxActivateDamageOverlay() {
    __state.damage_overlay_timer += 1.5f;
    if (__state.damage_overlay_timer > 60.f) {
        __state.damage_overlay_timer = 60.f;
    }
    if (__state.damage_overlay_play == 0) {
        __state.damage_overlay_timer2 = 0.15f;
        __state.damage_overlay_timer += 1.f;
        _ntRendererResetAnimation(__state.damage_overlay_anim);
    }
    __state.damage_overlay_play = 1;
}
void _fGfxStopDamageOverlay() {
    __state.damage_overlay_timer = 0.f;
    __state.damage_overlay_timer2 = 0.f;
    __state.damage_overlay_play = 0;
    if (__state.damage_overlay_anim) __state.damage_overlay_anim->current_value = 0;
}
void _fGfxDrawDamageOverlay() {
    const Vector2 surface_size = {
        (float)__state.framebuffer.texture.width,
        (float)__state.framebuffer.texture.height,
    };

    if (__state.damage_overlay.width <= 0 || __state.damage_overlay.height <= 0) return;

    const Vector2 scale_factor = {
        (float)__state.framebuffer.texture.width / (float)__state.damage_overlay.width,
        (float)__state.framebuffer.texture.height / (float)__state.damage_overlay.height
    };

    if (__state.damage_overlay_play && __state.damage_overlay_anim) {
        __state.damage_overlay_anim->delta = GetFrameTime();
        _ntRendererUpdateAnimation(__state.damage_overlay_anim);

        float scale = _ntRendererGetAnimationResult(__state.damage_overlay_anim, 1) * ((scale_factor.x + scale_factor.y) / 2.f);
        const Vector2 texture_size = {
            (float)__state.damage_overlay.width * scale,
            (float)__state.damage_overlay.height * scale,
        };
        const Vector2 center = {
            (surface_size.x - texture_size.x) / 2,
            (surface_size.y - texture_size.y) / 2,
        };

        const float ratio = 1 / 0.15f;
        const float pos = 1.f - _fSanitizeFloat(__state.damage_overlay_timer2 * ratio, 0, 1);

        Color overlay_color = WHITE;
        overlay_color.a = (unsigned char)(255.f * pos);

        DrawTextureEx(__state.damage_overlay, center, 0.f, scale, overlay_color);

        const float delta = __state.damage_overlay_anim->delta;
        __state.damage_overlay_timer2 -= delta;
        __state.damage_overlay_timer -= delta;
        if (__state.damage_overlay_timer2 < 0.f) {
            __state.damage_overlay_timer2 = 0;
        }
        if (__state.damage_overlay_timer < 0.f) {
            __state.damage_overlay_timer = 0;
            __state.damage_overlay_timer2 = 1;
            __state.damage_overlay_play = 0;
        }

        Color col = BLACK;
        col.a = (unsigned char)(255.f * pos * 0.25f);

        DrawRectangleV((Vector2) { 0, 0 }, surface_size, col);
    }
    else {
        if (__state.damage_overlay_timer2 > 0) {
            const float pos = __state.damage_overlay_timer2;
            float scale = _ntRendererGetAnimationResult(__state.damage_overlay_anim, 1) * ((scale_factor.x + scale_factor.y) / 2.f);

            Color col = BLACK;
            col.a = (unsigned char)(255.f * pos * 0.25f);

            __state.damage_overlay_timer2 -= GetFrameTime();

            const Vector2 texture_size = {
                (float)__state.damage_overlay.width * scale,
                (float)__state.damage_overlay.height * scale
            };
            const Vector2 center = {
                (surface_size.x - texture_size.x) / 2,
                (surface_size.y - texture_size.y) / 2,
            };

            Color overlay_color = WHITE;
            overlay_color.a = (unsigned char)(255.f * pos);

            DrawTextureEx(__state.damage_overlay, center, 0.f, scale, overlay_color);
            DrawRectangleV((Vector2) { 0, 0 }, surface_size, col);
        }
    }
}
