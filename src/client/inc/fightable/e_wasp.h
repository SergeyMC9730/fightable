
//          Sergei Baigerov 2024 - 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <fightable/entity.h>
#include <rsb/rsb_array_gen.h>

#ifdef __cplusplus
extern "C" {
#endif

struct fewasp_state_entry {
    struct fentity *locked_entity;

    unsigned char played_effect_1;
    unsigned char played_effect_2;
    unsigned char played_effect_3;
};

RSB_ARRAY_DEF_GEN(struct fewasp_state_entry, _wasp_se);

struct fentity_wasp {
	struct fentity base;
	Texture2D main_texture;
	Texture2D texture2;
	Image temp_img;
	Image temp_img2;

	Image tile1;
	Image tile2;

	double live_time;
	unsigned long long live_frames;

	rsb_array__wasp_se *trapped_entities;
};

void _feWaspInit(struct fentity_wasp* instance);
void _feWaspDraw(struct fentity_wasp* instance);
void _feWaspCleanup(struct fentity_wasp* instance);
void _feWaspUpdate(struct fentity_wasp* instance);

#ifdef __cplusplus
}
#endif
