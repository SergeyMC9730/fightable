
//          Sergei Baigerov 2024 - 2026.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

struct fentity;

struct fentity *_fEntitySpawnTempLabel(struct fentity *instance, char *data);
struct fentity *_fEntitySpawnEnemyBullet(struct fentity *instance, unsigned char level);
struct fentity *_fEntitySpawnPlayerBullet(struct fentity *instance, unsigned char level);

#ifdef __cplusplus
}
#endif
