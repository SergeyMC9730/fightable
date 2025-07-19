
//          Sergei Baigerov 2024 - 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include <fightable/string.h>
#include <fightable/text_instance.h>
#include <string.h>

RSB_ARRAY_IMPL_GEN(struct ftext_instance_entry, _tie);

void _fTextInstanceProcessEntry(struct ftext_instance *instance, struct ftext_instance_entry *e) {
    if (!instance || !e || !e->text) return;

    switch (e->command) {
        case TC_COLOR_GRADIENT: {
            switch (instance->used_font_backend) {
                case TIB_FIGHTABLE: {
                    if (!instance->applicable_fightable_font) break;

                    e->prerendered_text = _fTextRenderGradientV(instance->applicable_fightable_font, e->text, e->color_a, e->color_b, 1);

                    break;
                }
                case TIB_RAYLIB: {
                    if (!IsFontValid(instance->applicable_raylib_font)) break;

                    e->prerendered_text = _fTextRenderGradientVRl(instance->applicable_raylib_font, instance->raylib_size, instance->raylib_spacing, e->text, e->color_a, e->color_b, 1);

                    break;
                }
            }

            break;
        }
        default: {}
    }

    if (!instance->text_elements) {
        instance->text_elements = RSBCreateArray_tie();
    }
    RSBAddElement_tie(instance->text_elements, *e);
}

Color _fTextInstanceStringToColor(const char *text) {
    if (!text) return WHITE;

    if (!strcmp(text, "lightgray")) return LIGHTGRAY;
    if (!strcmp(text, "gray")) return GRAY;
    if (!strcmp(text, "darkgray")) return DARKGRAY;
    if (!strcmp(text, "yellow")) return YELLOW;
    if (!strcmp(text, "gold")) return GOLD;
    if (!strcmp(text, "orange")) return ORANGE;
    if (!strcmp(text, "pink")) return PINK;
    if (!strcmp(text, "red")) return RED;
    if (!strcmp(text, "maroon")) return MAROON;
    if (!strcmp(text, "green")) return GREEN;
    if (!strcmp(text, "lime")) return LIME;
    if (!strcmp(text, "darkgreen")) return DARKGREEN;
    if (!strcmp(text, "skyblue")) return SKYBLUE;
    if (!strcmp(text, "blue")) return BLUE;
    if (!strcmp(text, "darkblue")) return DARKBLUE;
    if (!strcmp(text, "purple")) return PURPLE;
    if (!strcmp(text, "violet")) return VIOLET;
    if (!strcmp(text, "darkpurple")) return DARKPURPLE;
    if (!strcmp(text, "beige")) return BEIGE;
    if (!strcmp(text, "brown")) return BROWN;
    if (!strcmp(text, "darkbrown")) return DARKBROWN;
    if (!strcmp(text, "white")) return WHITE;
    if (!strcmp(text, "black")) return BLACK;
    if (!strcmp(text, "blank")) return BLANK;
    if (!strcmp(text, "magenta")) return MAGENTA;
    if (!strcmp(text, "raywhite")) return RAYWHITE;

    return WHITE;
}

void _fTextInstanceCreatePrivate(struct ftext_instance *instance) {
    enum ftext_instance_command current_command = TC_NONE;
    Color current_text_colorA = WHITE;
    Color current_text_colorB = WHITE;

    size_t text_len = strlen(instance->original_string);
    char *temp_buffer = (char *)malloc(text_len + 1);
    size_t temp_buffer_i = 0;
    memset(temp_buffer, 0, text_len + 1);

    char *current_text_start = instance->original_string + 0;

    for (size_t i = 0; i < text_len; i++) {
        char c = instance->original_string[i];

        // current_text_start = instance->original_string + i;

        if (c == '<') {
            if (temp_buffer_i > 0) {
                struct ftext_instance_entry e = {};
                e.text = _fCopyStringWithLen(temp_buffer, strlen(temp_buffer));
                e.color_a = current_text_colorA;
                e.color_b = current_text_colorB;
                e.command = current_command;
                _fTextInstanceProcessEntry(instance, &e);

                temp_buffer_i = 0;
                current_text_start = instance->original_string + i;
            }

            if (i + 1 < text_len && instance->original_string[i + 1] == '/') {
                current_command = TC_NONE;
                i++;
                continue;
            }

            if (current_command == TC_UNKNOWN) continue;

            current_command = TC_UNKNOWN;
            memset(temp_buffer, 0, text_len + 1);
            temp_buffer_i = 0;

            for (size_t j = i + 1; j < text_len; j++) {
                if (instance->original_string[j] == '>') break;

                temp_buffer[temp_buffer_i++] = instance->original_string[j];
            }

            if (strlen(temp_buffer) == 0) continue;

            char command_c = temp_buffer[0];

            switch (command_c) {
                case 'c': {
                    if (strlen(temp_buffer) < 3) {
                        TraceLog(LOG_WARNING, "Text command '%c' is incomplete", command_c);
                        break;
                    }

                    rsb_array__pchar *splitted = _fSplitString(temp_buffer + 1, ',');

                    if (splitted->added_elements == 1) {
                        current_command = TC_COLOR_BASIC_TINT;
                        current_text_colorA = _fTextInstanceStringToColor(RSBGetAtIndex_pchar(splitted, 0));
                    } else if (splitted->added_elements > 1) {
                        current_command = TC_COLOR_GRADIENT;
                        current_text_colorA = _fTextInstanceStringToColor(RSBGetAtIndex_pchar(splitted, 0));
                        current_text_colorB = _fTextInstanceStringToColor(RSBGetAtIndex_pchar(splitted, 1));
                    }

                    _fCleanupSplittedString(splitted);

                    break;
                }
                default: {
                    TraceLog(LOG_WARNING, "Unknown text command '%c'", command_c);
                    break;
                }
            }

            i += strlen(temp_buffer);

            memset(temp_buffer, 0, text_len + 1);
            temp_buffer_i = 0;

            current_text_start = instance->original_string + i;

            continue;
        } else if (c == '/') {
            if (current_command == TC_UNKNOWN) {
                current_command = TC_NONE;
            } else {
                temp_buffer[temp_buffer_i++] = instance->original_string[i];
            }
            continue;
        } else if (c == '>') {
            continue;
        }

        temp_buffer[temp_buffer_i++] = instance->original_string[i];
    }

    struct ftext_instance_entry e = {};

    e.text = _fCopyStringWithLen(current_text_start, strlen(temp_buffer));
    e.color_a = current_text_colorA;
    e.color_b = current_text_colorB;
    e.command = current_command;

    _fTextInstanceProcessEntry(instance, &e);
}

struct ftext_instance _fTextInstanceCreateWithTextMan(const char *text, struct ftext_manager *man) {
    struct ftext_instance instance = {};
    instance.used_font_backend = TIB_FIGHTABLE;
    instance.applicable_fightable_font = man;

    if (!instance.applicable_fightable_font) {
        TraceLog(LOG_ERROR, "Text manager is not loaded");

        return instance;
    }
    if (!text) {
        TraceLog(LOG_ERROR, "Text is not loaded");
        return instance;
    }

    instance.original_string = _fCopyString(text);

    _fTextInstanceCreatePrivate(&instance);

    return instance;
}

struct ftext_instance _fTextInstanceCreateWithFont(const char *text, RLFont font, float size, float spacing) {
    struct ftext_instance instance = {};
    instance.used_font_backend = TIB_RAYLIB;
    instance.applicable_raylib_font = font;
    instance.raylib_size = size;
    instance.raylib_spacing = spacing;

    if (!text) {
        TraceLog(LOG_ERROR, "Text is not loaded");
        return instance;
    }

    instance.original_string = _fCopyString(text);

    _fTextInstanceCreatePrivate(&instance);

    return instance;
}

void _fTextInstanceDestroy(struct ftext_instance *instance) {
    if (!instance) return;

    if (instance->original_string) {
        free(instance->original_string);
        instance->original_string = NULL;
    }
    if (instance->text_elements) {
        for (size_t i = 0; i < instance->text_elements->added_elements; i++) {
            struct ftext_instance_entry entry = RSBGetAtIndex_tie(instance->text_elements, i);

            if (IsTextureValid(entry.prerendered_text)) {
                UnloadTexture(entry.prerendered_text);
            }
            if (entry.text) {
                free(entry.text);
            }
        }

        RSBDestroy_tie(instance->text_elements);
        instance->text_elements = NULL;
    }
}

void _fTextInstanceDrawPrivateF(struct ftext_instance *instance, struct ftext_instance_entry entry, Vector2 *position) {
    if (!instance || instance->applicable_fightable_font || !entry.text) return;

    IVector2 m = _fTextMeasure(instance->applicable_fightable_font, entry.text);
    Color color = WHITE;

    if (entry.command == TC_COLOR_BASIC_TINT) color = entry.color_a;

    _fTextDraw(instance->applicable_fightable_font, entry.text, (IVector2){(int)position->x, (int)position->y}, color, 1);
    position->x += m.x;
    // position->y += m.y;
}
void _fTextInstanceDrawPrivateR(struct ftext_instance *instance, struct ftext_instance_entry entry, Vector2 *position) {
    if (!instance || !IsFontValid(instance->applicable_raylib_font) || !entry.text) return;

    Vector2 m = MeasureTextEx(instance->applicable_raylib_font, entry.text, instance->raylib_size, instance->raylib_spacing);
    Color color = WHITE;

    if (entry.command == TC_COLOR_BASIC_TINT) color = entry.color_a;

    RlDrawTextEx(instance->applicable_raylib_font, entry.text, *position, instance->raylib_size, instance->raylib_spacing, color);
    position->x += m.x;
}

void _fTextInstanceDraw(struct ftext_instance *instance, Vector2 position) {
    if (!instance || !instance->text_elements) return;

    Vector2 cur_position = position;
    void (*draw_private)(struct ftext_instance*, struct ftext_instance_entry, Vector2*) = NULL;

    switch (instance->used_font_backend) {
        case TIB_FIGHTABLE: {
            draw_private = _fTextInstanceDrawPrivateF;
            break;
        }
        case TIB_RAYLIB: {
            draw_private = _fTextInstanceDrawPrivateR;
            break;
        }
    }

    if (!draw_private) return;

    for (size_t i = 0; i < instance->text_elements->added_elements; i++) {
        struct ftext_instance_entry entry = RSBGetAtIndex_tie(instance->text_elements, i);

        switch (entry.command) {
            case TC_NONE:
            case TC_UNKNOWN:
            case TC_COLOR_BASIC_TINT: {
                draw_private(instance, entry, &cur_position);
                break;
            }
            case TC_COLOR_GRADIENT: {
                if (!IsTextureValid(entry.prerendered_text)) break;

                DrawTexture(entry.prerendered_text, cur_position.x, cur_position.y, WHITE);
                cur_position.x += entry.prerendered_text.width;

                break;
            }
        }
    }
}
