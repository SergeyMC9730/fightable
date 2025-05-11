
//          Sergei Baigerov 2024 - 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include <fightable/notif_object.h>
#include <fightable/string.h>
#include <nt5emul/renderer_animation.h>
#include <nt5emul/renderer_keyframe.h>
#include <stdlib.h>
#include <string.h>

#define MOVE_ANIM_X_ID 0
#define MOVE_ANIM_Y_ID 1

#define POPUP_SAFE_AREA 10

struct fnotif_object *_fNotifObjectCreate(const char *content) {
    struct fnotif_object *o = (struct fnotif_object *)malloc(sizeof(struct fnotif_object));
    memset(o, 0, sizeof(struct fnotif_object));

    RLFont font = GetFontDefault();

    o->copied_message = _fCopyString(content);
    if (!o->copied_message) {
        o->box.width = 50;
        o->box.height = font.baseSize + (POPUP_SAFE_AREA * 2);
    } else {
        Vector2 sz = MeasureTextEx(font, o->copied_message, 20.f, 1.f);
        o->box.width = sz.x + (POPUP_SAFE_AREA * 2);
        o->box.height = sz.y + (POPUP_SAFE_AREA * 2);
    }

    return o;
}
void _fNotifObjectDestroy(struct fnotif_object *o, unsigned char object_allocated) {
    if (!o) return;

    if (o->copied_message) free((void *)o->copied_message); o->copied_message = NULL;
    _ntRendererUnloadAnimation(o->open_anim); o->open_anim = NULL;
    _ntRendererUnloadAnimation(o->close_anim); o->close_anim = NULL;
    _ntRendererUnloadAnimation(o->move_anim); o->move_anim = NULL;

    o->box = (RLRectangle){};

    if (object_allocated) free(o);
}

void _fNotifObjectSetPosV(struct fnotif_object *o, Vector2 p) {
    _fNotifObjectApplyPosAnimation(o, p, 0);
}
void _fNotifObjectSetPos(struct fnotif_object *o, float x, float y) {
    _fNotifObjectSetPosV(o, (Vector2){x, y});
}

void _fNotifObjectApplyOpenAnimation(struct fnotif_object *o) {
    if (!o) return;

    _ntRendererUnloadAnimation(o->open_anim);
    o->open_anim = (struct renderer_animation *)MemAlloc(sizeof(struct renderer_animation));
    o->open_anim->count = 1;
    o->open_anim->early_value = -o->box.height;
    o->open_anim->valid = 1;
    _ntRendererResetAnimation(o->open_anim);
    o->open_anim->keyframes = (struct renderer_keyframe *)MemAlloc(sizeof(struct renderer_keyframe) * o->open_anim->count);
    struct renderer_keyframe *k = o->open_anim->keyframes + 0;
    k->easing = TOOutExpo;
    k->ending_value = -o->open_anim->early_value;
    k->length = 1.f;

    o->closed = 0;
}
void _fNotifObjectApplyCloseAnimation(struct fnotif_object *o) {
    if (!o) return;

    _ntRendererUnloadAnimation(o->close_anim);
    o->close_anim = (struct renderer_animation *)MemAlloc(sizeof(struct renderer_animation));
    o->close_anim->count = 1;
    o->close_anim->early_value = 0;
    o->close_anim->valid = 1;
    _ntRendererResetAnimation(o->close_anim);
    o->close_anim->keyframes = (struct renderer_keyframe *)MemAlloc(sizeof(struct renderer_keyframe) * o->close_anim->count);
    struct renderer_keyframe *k = o->close_anim->keyframes + 0;
    k->easing = TOInExpo;
    k->ending_value = -o->box.height;
    k->length = 1.f;
}
double _fNotifObjectGetFinalFromAnim(struct renderer_animation *anim) {
    if (!anim || !anim->keyframes) return 0;

    double r = 0;

    for (int i = 0; i < anim->count; i++) {
        struct renderer_keyframe k = anim->keyframes[i];
        r += k.ending_value;
    }

    return r;
}
void _fNotifObjectApplyPosAnimation(struct fnotif_object *o, Vector2 offset, unsigned char offset_is_relative) {
    if (!o) return;

    if (!offset_is_relative) {
        offset.x -= o->box.x;
        offset.y -= o->box.y;
    }

    if (offset.x == 0.f && offset.y == 0.f) return;

    struct renderer_animation *l = NULL;
    if (o->move_anim) {
        l = o->move_anim->linked_animation;

        o->box.x += _fNotifObjectGetFinalFromAnim(_ntRendererGetEmbeddedAnimation(o->move_anim, MOVE_ANIM_X_ID));
        o->box.y += _fNotifObjectGetFinalFromAnim(_ntRendererGetEmbeddedAnimation(o->move_anim, MOVE_ANIM_Y_ID));
    }

    enum renderer_tweak_type tweak = TOOutQuint;
    double time = 1;

    {
        _ntRendererUnloadAnimation(o->move_anim);
        o->move_anim = (struct renderer_animation *)MemAlloc(sizeof(struct renderer_animation));
        o->move_anim->anim_id = MOVE_ANIM_X_ID;
        o->move_anim->count = 1;
        o->move_anim->early_value = 0;
        o->move_anim->valid = 1;
        _ntRendererResetAnimation(o->move_anim);
        o->move_anim->keyframes = (struct renderer_keyframe *)MemAlloc(sizeof(struct renderer_keyframe) * o->move_anim->count);
        struct renderer_keyframe *k = o->move_anim->keyframes + 0;
        k->easing = tweak;
        k->ending_value = offset.x;
        k->length = time;
    }

    {
        _ntRendererUnloadAnimation(l);
        l = (struct renderer_animation *)MemAlloc(sizeof(struct renderer_animation));
        l->anim_id = MOVE_ANIM_Y_ID;
        l->count = 1;
        l->early_value = 0;
        l->valid = 1;
        _ntRendererResetAnimation(l);
        l->keyframes = (struct renderer_keyframe *)MemAlloc(sizeof(struct renderer_keyframe) * l->count);
        struct renderer_keyframe *k = l->keyframes + 0;
        k->easing = tweak;
        k->ending_value = offset.y;
        k->length = time;
        o->move_anim->linked_animation = l;
    }
}

void _fNotifObjectDraw(struct fnotif_object *o) {
    if (!o || o->closed) return;

    Vector2 offset = {};

    float delta = GetFrameTime();
    if (o->open_anim) {
        o->open_anim->delta = delta;
        _ntRendererUpdateAnimation(o->open_anim);
    }
    if (o->close_anim) {
        o->close_anim->delta = delta;
        _ntRendererUpdateAnimation(o->close_anim);
    }
    if (o->move_anim) {
        o->move_anim->delta = delta;
        _ntRendererUpdateAnimation(o->move_anim);
    }

    if (o->open_anim) {
        offset.y += o->open_anim->current_value;
        if (o->open_anim->completed) {
            _ntRendererUnloadAnimation(o->open_anim);
            o->open_anim = NULL;
        }
    }
    if (o->close_anim) {
        offset.y += o->close_anim->current_value;
        if (o->close_anim->completed) {
            _ntRendererUnloadAnimation(o->close_anim);
            o->close_anim = NULL;
            o->closed = 1;
        }
    }
    if (o->move_anim) {
        if (o->move_anim->completed) {
            o->box.x += _fNotifObjectGetFinalFromAnim(_ntRendererGetEmbeddedAnimation(o->move_anim, MOVE_ANIM_X_ID));
            o->box.y += _fNotifObjectGetFinalFromAnim(_ntRendererGetEmbeddedAnimation(o->move_anim, MOVE_ANIM_Y_ID));

            _ntRendererUnloadAnimation(o->move_anim->linked_animation); o->move_anim->linked_animation = NULL;
            _ntRendererUnloadAnimation(o->move_anim); o->move_anim = NULL;
        } else {
            offset.x -= _ntRendererGetAnimationResult(o->move_anim, MOVE_ANIM_X_ID);
            offset.y -= _ntRendererGetAnimationResult(o->move_anim, MOVE_ANIM_Y_ID);
        }
    }

    // TraceLog(LOG_INFO, "%f %f", offset.x, offset.y);

    DrawRectangle(o->box.x + offset.x, o->box.y + offset.y, o->box.width, o->box.height, (Color){0,0,0,200});
    DrawRectangleLinesEx((RLRectangle){o->box.x + offset.x, o->box.y + offset.y, o->box.width, o->box.height}, 2.f, YELLOW);

    if (!o->copied_message) return;

    RLFont font = GetFontDefault();
    Vector2 sz = MeasureTextEx(font, o->copied_message, 20.f, 1.f);

    Vector2 center = {(o->box.width - sz.x) / 2.f, (o->box.height - sz.y) / 2.f};
    RlDrawTextEx(font, o->copied_message, (Vector2){center.x + o->box.x + offset.x + 2, center.y + o->box.y + offset.y + 2}, 20.f, 1.f, BLACK);
    RlDrawTextEx(font, o->copied_message, (Vector2){center.x + o->box.x + offset.x, center.y + o->box.y + offset.y}, 20.f, 1.f, WHITE);
}
