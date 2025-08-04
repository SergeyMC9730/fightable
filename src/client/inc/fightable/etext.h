#pragma once

#include <fightable/entity.h>
#include <fightable/multiline_text_instance.h>

#ifdef __cplusplus
extern "C" {
#endif

struct fentity_text {
	struct fentity base;
	struct fmultiline_text_instance *text;
};

void _feTextInit(struct fentity_text* instance, const char *text);
void _feTextDraw(struct fentity_text* instance);
void _feTextCleanup(struct fentity_text* instance);

#ifdef __cplusplus
}
#endif
