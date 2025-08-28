
//          Sergei Baigerov 2024 - 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <fightable/entity.h>

#ifdef __cplusplus
extern "C" {
#endif

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
};

void _feWaspInit(struct fentity_wasp* instance);
void _feWaspDraw(struct fentity_wasp* instance);
void _feWaspCleanup(struct fentity_wasp* instance);

#ifdef __cplusplus
}
#endif
