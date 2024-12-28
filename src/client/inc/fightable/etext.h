#pragma once

#include <fightable/entity.h>

#ifdef __cplusplus
extern "C" {
#endif

struct fentity_text {
	struct fentity base;
	char* text;
};

void _feTextInit(struct fentity_text* instance, const char *text);
void _feTextDraw(struct fentity_text* instance);
void _feTextCleanup(struct fentity_text* instance);

#ifdef __cplusplus
}
#endif