
//          Sergei Baigerov 2024 - 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <fraylib.h>
#include <nt5emul/renderer_animation.h>

struct fnotif_object {
    const char *copied_message;
    RLRectangle box;
    struct renderer_animation *open_anim;
    struct renderer_animation *close_anim;
    struct renderer_animation *move_anim;
    unsigned char closed;
};

#ifdef __cplusplus
extern "C" {
#endif

struct fnotif_object *_fNotifObjectCreate(const char *content);
void _fNotifObjectSetPosV(struct fnotif_object *o, Vector2 p);
void _fNotifObjectSetPos(struct fnotif_object *o, float x, float y);
void _fNotifObjectDestroy(struct fnotif_object *o, unsigned char object_allocated);
void _fNotifObjectApplyOpenAnimation(struct fnotif_object *o);
void _fNotifObjectApplyCloseAnimation(struct fnotif_object *o);
void _fNotifObjectApplyPosAnimation(struct fnotif_object *o, Vector2 offset, unsigned char offset_is_relative);
void _fNotifObjectDraw(struct fnotif_object *o);

#ifdef __cplusplus
}
#endif
