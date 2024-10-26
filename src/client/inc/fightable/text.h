#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <fightable/tilemap.h>

struct ftext_manager {
    struct ftilemap tilemap;

    unsigned int char_padding;
    IVector2 *char_mapping;
};

struct ftext_manager _fTextLoadDefault();

IVector2 _fTextMeasure(struct ftext_manager *man, const char *text);
void _fTextDraw(struct ftext_manager *man, const char *text, IVector2 pos, Color color, unsigned char with_shadow);

Texture2D _fTextRenderGradientV(struct ftext_manager *man, const char *text, Color top, Color bottom, unsigned char with_shadow);

#ifdef __cplusplus
}
#endif
