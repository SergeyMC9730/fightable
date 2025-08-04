
//          Sergei Baigerov 2024 - 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)
//
#include <fightable/string.h>
#include <fightable/text_instance.hpp>
#include <string.h>
#include <fightable/generic_tools.hpp>

void _fTextInstanceAddEntry(struct ftext_instance *instance, struct ftext_instance_entry e) {
    instance->text_elements.push_back(e);
}

void _fTextInstanceProcessEntry(struct ftext_instance *instance, struct ftext_instance_entry *e) {
    if (!instance || !e) return;

    switch (e->command) {
        case TC_COLOR_GRADIENT: {
            switch (instance->used_font_backend) {
                case TIB_FIGHTABLE: {
                    if (!instance->applicable_fightable_font) break;

                    e->prerendered_text = _fTextRenderGradientV(instance->applicable_fightable_font, e->text.c_str(), e->color_a, e->color_b, 1);

                    break;
                }
                case TIB_RAYLIB: {
                    if (!IsFontValid(instance->applicable_raylib_font)) break;

                    e->prerendered_text = _fTextRenderGradientVRl(instance->applicable_raylib_font, instance->raylib_size, instance->raylib_spacing, e->text.c_str(), e->color_a, e->color_b, 1);

                    break;
                }
            }

            break;
        }
        default: {}
    }

    _fTextInstanceAddEntry(instance, *e);
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

    size_t text_len = instance->original_string.length();
    char *temp_buffer = (char *)MemAlloc(text_len);
    size_t temp_buffer_i = 0;

    char *tmp;
    const char *current_text_start = instance->original_string.c_str() + 0;

    for (size_t i = 0; i < text_len; i++) {
        char c = instance->original_string[i];

        if (c == '<') {
            if (temp_buffer_i > 0) {
                struct ftext_instance_entry e = {};
                tmp = _fCopyStringWithLen(current_text_start, strlen(temp_buffer));
                e.text = tmp;
                e.color_a = current_text_colorA;
                e.color_b = current_text_colorB;
                e.command = current_command;
                _fTextInstanceProcessEntry(instance, &e);

                free(tmp);

                temp_buffer_i = 0;
                current_text_start = instance->original_string.c_str() + i;
            }

            if (i + 1 < text_len && instance->original_string[i + 1] == '/') {
                current_command = TC_NONE;
                i++;
                continue;
            }

            if (current_command == TC_UNKNOWN) continue;

            current_command = TC_UNKNOWN;
            memset(temp_buffer, 0, text_len);
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

                    // rsb_array__pchar *splitted = _fSplitString(temp_buffer + 1, ',');
                    auto splitted = GenericTools::splitString(temp_buffer + 1, ',');

                    if (splitted.size() == 1) {
                        current_command = TC_COLOR_BASIC_TINT;
                        current_text_colorA = _fTextInstanceStringToColor(splitted[0].c_str());
                    } else if (splitted.size() > 1) {
                        current_command = TC_COLOR_GRADIENT;
                        current_text_colorA = _fTextInstanceStringToColor(splitted[0].c_str());
                        current_text_colorB = _fTextInstanceStringToColor(splitted[1].c_str());
                    }

                    break;
                }
                default: {
                    TraceLog(LOG_WARNING, "Unknown text command '%c'", command_c);
                    break;
                }
            }

            i += strlen(temp_buffer);

            memset(temp_buffer, 0, text_len);
            temp_buffer_i = 0;

            current_text_start = instance->original_string.c_str() + i + 2;

            continue;
        } else if (c == '>') {
            continue;
        }

        temp_buffer[temp_buffer_i++] = instance->original_string[i];
    }

    struct ftext_instance_entry e = {};

    tmp = _fCopyStringWithLen(current_text_start, strlen(temp_buffer));
    e.text = tmp;
    e.color_a = current_text_colorA;
    e.color_b = current_text_colorB;
    e.command = current_command;

    free(tmp);

    _fTextInstanceProcessEntry(instance, &e);
}

struct ftext_instance *_fTextInstanceCreateWithTextMan(const char *text, struct ftext_manager *man) {
    struct ftext_instance *instance = new struct ftext_instance;
    instance->used_font_backend = TIB_FIGHTABLE;
    instance->applicable_fightable_font = man;

    if (!instance->applicable_fightable_font) {
        TraceLog(LOG_ERROR, "Text manager is not loaded");

        return instance;
    }
    if (!text) {
        TraceLog(LOG_ERROR, "Text is not loaded");
        return instance;
    }

    instance->original_string = text;

    _fTextInstanceCreatePrivate(instance);

    return instance;
}

struct ftext_instance *_fTextInstanceCreateWithFont(const char *text, RLFont font, float size, float spacing) {
    struct ftext_instance *instance = new struct ftext_instance;
    instance->used_font_backend = TIB_RAYLIB;
    instance->applicable_raylib_font = font;
    instance->raylib_size = size;
    instance->raylib_spacing = spacing;

    if (!text) {
        TraceLog(LOG_ERROR, "Text is not loaded");
        return instance;
    }

    instance->original_string = text;

    _fTextInstanceCreatePrivate(instance);

    return instance;
}

void _fTextInstanceDestroy(struct ftext_instance *instance) {
    if (!instance) return;

    for (const auto &entry : instance->text_elements) {
        if (IsTextureValid(entry.prerendered_text)) {
            UnloadTexture(entry.prerendered_text);
        }
    }

    delete instance;
}

void _fTextInstanceDrawPrivateF(struct ftext_instance *instance, const struct ftext_instance_entry &entry, Vector2 *position) {
    if (!instance || instance->applicable_fightable_font) return;

    IVector2 m = _fTextMeasure(instance->applicable_fightable_font, entry.text.c_str());
    Color color = WHITE;

    if (entry.command == TC_COLOR_BASIC_TINT) color = entry.color_a;

    _fTextDraw(instance->applicable_fightable_font, entry.text.c_str(), (IVector2){(int)position->x, (int)position->y}, color, 1);
    position->x += m.x;
}
void _fTextInstanceDrawPrivateR(struct ftext_instance *instance, const struct ftext_instance_entry &entry, Vector2 *position) {
    if (!instance || !IsFontValid(instance->applicable_raylib_font)) return;

    Vector2 m = MeasureTextEx(instance->applicable_raylib_font, entry.text.c_str(), instance->raylib_size, instance->raylib_spacing);
    Color color = WHITE;

    if (entry.command == TC_COLOR_BASIC_TINT) color = entry.color_a;

    RlDrawTextEx(instance->applicable_raylib_font, entry.text.c_str(), *position, instance->raylib_size, instance->raylib_spacing, color);
    position->x += m.x;
}

void _fTextInstanceDraw(struct ftext_instance *instance, Vector2 position) {
    if (!instance) return;

    Vector2 cur_position = position;
    void (*draw_private)(struct ftext_instance*, const struct ftext_instance_entry&, Vector2*) = NULL;

    // TraceLog(LOG_INFO, "TEXT ENTRIES: %ld", instance->text_elements.size());

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

    for (const auto &entry : instance->text_elements) {
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

Vector2 _fTextInstanceGetSize(struct ftext_instance *instance) {
    if (!instance) return (Vector2){};

    Vector2 cur_position = (Vector2){};

    for (const auto &entry : instance->text_elements) {
        switch (entry.command) {
            case TC_NONE:
            case TC_UNKNOWN:
            case TC_COLOR_BASIC_TINT: {
                switch (instance->used_font_backend) {
                    case TIB_FIGHTABLE: {
                        IVector2 sz = _fTextMeasure(instance->applicable_fightable_font, entry.text.c_str());
                        cur_position.x += (float)sz.x;
                        if (cur_position.y < (float)sz.y) {
                            cur_position.y = (float)sz.y;
                        }
                        break;
                    }
                    case TIB_RAYLIB: {
                        Vector2 sz = MeasureTextEx(instance->applicable_raylib_font, entry.text.c_str(), instance->raylib_size, instance->raylib_spacing);
                        cur_position.x += sz.x;
                        if (cur_position.y < sz.y) {
                            cur_position.y = sz.y;
                        }
                        break;
                    }
                }
                break;
            }
            case TC_COLOR_GRADIENT: {
                if (!IsTextureValid(entry.prerendered_text)) break;

                cur_position.x += entry.prerendered_text.width;
                if (cur_position.y < (float)entry.prerendered_text.height) {
                    cur_position.y = (float)entry.prerendered_text.height;
                }

                break;
            }
        }
    }

    return cur_position;
}

const char *_fTextInstanceGetText(struct ftext_instance *instance) {
    if (!instance) return NULL;

    return instance->original_string.c_str();
}
