/*
    nt5 -- Windows XP simulator.
    Copyright (C) 2024  Sergei Baigerov

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU Affero General Public License as published
    by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.

    Contact Sergei Baigerov -- @dogotrigger in Discord
*/

#define DEBUG 1

#include <nt5emul/renderer_keyframe.h>
#include <nt5emul/renderer_animation.h>
#include <nt5emul/renderer_ease.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <raylib.h>

static char __ntRendererAnimTreeText[64] = {};

void _ntRendererPrintAnimationTree(struct renderer_animation *animation) {
    if (animation == NULL) return;

    size_t tree_level_idx = strlen(__ntRendererAnimTreeText);

    TraceLog(LOG_INFO, "%sbegin anim_id %d:", __ntRendererAnimTreeText, animation->anim_id);

    if (!animation->valid) {
        TraceLog(LOG_INFO, "%s #invalid animation#", __ntRendererAnimTreeText);

        if (tree_level_idx != 0) {
            __ntRendererAnimTreeText[--tree_level_idx] = 0;
        }

        return;
    }

    TraceLog(LOG_INFO, "%s - initial value: %f", __ntRendererAnimTreeText, (float)animation->starting_value);
    TraceLog(LOG_INFO, "%s - current value: %f (local: %f)", __ntRendererAnimTreeText, (float)animation->current_value, (float)animation->local_current_value);
    TraceLog(LOG_INFO, "%s - final   value: %f", __ntRendererAnimTreeText, (float)animation->final_value);

    TraceLog(LOG_INFO, "\n%s - time spent: %f", __ntRendererAnimTreeText, (float)animation->time);
    TraceLog(LOG_INFO, "%s - time spent on keyframe: %f", __ntRendererAnimTreeText, (float)animation->itime);
    TraceLog(LOG_INFO, "%s - delta time: %f", __ntRendererAnimTreeText, (float)animation->delta);

    TraceLog(LOG_INFO, "\n%s - completed: %d (local %d)", __ntRendererAnimTreeText, animation->completed, animation->completed_local);

    TraceLog(LOG_INFO, "\n%s - current keyframe: %d", __ntRendererAnimTreeText, animation->current_keyframe);
    TraceLog(LOG_INFO, "%s - keyframes (%d):", __ntRendererAnimTreeText, animation->count);

    for (unsigned int i = 0; i < animation->count; i++) {
        struct renderer_keyframe keyframe = animation->keyframes[i];

        TraceLog(LOG_INFO, "%s  %d: (v)%f; (e)%d; (l)%f", __ntRendererAnimTreeText, i, (float)keyframe.ending_value, keyframe.easing, (float)keyframe.length);
    }

    if (animation->linked_animation != NULL) {
        TraceLog(LOG_INFO, "\n%s - linked animation:", __ntRendererAnimTreeText);

        if (tree_level_idx == sizeof(__ntRendererAnimTreeText) - 1) {
            TraceLog(LOG_INFO, "%s   cannot be displayed: too big tree", __ntRendererAnimTreeText);
        } else {
            __ntRendererAnimTreeText[tree_level_idx + 0] = ' ';

            _ntRendererPrintAnimationTree(animation->linked_animation);
        }
    } else if (animation->linked_animation == NULL && animation->influenced) {
        TraceLog(LOG_INFO, "\n%s ?? animation's influenced by NULL", __ntRendererAnimTreeText);
    }

    if (tree_level_idx != 0) {
        __ntRendererAnimTreeText[--tree_level_idx] = 0;
    }
}

void _ntRendererResetAnimTree() {
    memset(__ntRendererAnimTreeText, 0, sizeof(__ntRendererAnimTreeText));
}
