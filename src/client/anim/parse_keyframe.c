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

#include <nt5emul/renderer_keyframe.h>
#include <cJSON.h>
#include <raylib.h>
#include <stdio.h>

struct renderer_keyframe _ntRendererLoadKeyframe(void *cjson_object_ptr) {
    cJSON *cjson_object = (cJSON *)cjson_object_ptr;
    struct renderer_keyframe frame = {0};

    // check if object is valid
    if (cjson_object == NULL) {
        // set keyframe lenght to an invalid value
        // and return this keyframe

        frame.length = -1;
        return frame;
    }

    // try to get length
    cJSON *tmp_obj = cJSON_GetObjectItem(cjson_object, "length");
    if (tmp_obj) {
        // check if length can be parsed or not
        if (cJSON_IsNumber(tmp_obj)) {
            frame.length = cJSON_GetNumberValue(tmp_obj);
        } else {
            TraceLog(LOG_INFO, "%s:%s: length is not a number (NaN)", __FILE_NAME__, __LINE__);
        }
    }

    // try to get ending value
    tmp_obj = cJSON_GetObjectItem(cjson_object, "ending_value");
    if (tmp_obj) {
        // check if ending value can be parsed or not
        if (cJSON_IsNumber(tmp_obj)) {
            frame.ending_value = cJSON_GetNumberValue(tmp_obj);
        } else {
            TraceLog(LOG_INFO, "%s:%s: ending_value is not a number (NaN)\n", __FILE_NAME__, __LINE__);
        }
    }

    // try to get easing
    tmp_obj = cJSON_GetObjectItem(cjson_object, "easing");
    if (tmp_obj) {
        // check if easing can be parsed or not
        if (cJSON_IsNumber(tmp_obj)) {
            renderer_tweak_type easing = (renderer_tweak_type)(cJSON_GetNumberValue(tmp_obj));

            // easing should not be set beyond the TOEnd
            frame.easing = easing % TOEnd;
        } else {
            TraceLog(LOG_INFO, "%s:%s: easing is not a number (NaN)\n", __FILE_NAME__, __LINE__);
        }
    }

    return frame;
}
