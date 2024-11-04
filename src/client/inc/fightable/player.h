#pragma once

#include <fightable/entity.h>

#ifdef __cplusplus
extern "C" {
#endif

struct felplayer {
	struct fentity base;
};

void _flPlayerInit(struct felplayer* instance);
void _flPlayerDraw(struct felplayer* instance);
void _flPlayerUpdate(struct felplayer* instance);

#ifdef __cplusplus
}
#endif