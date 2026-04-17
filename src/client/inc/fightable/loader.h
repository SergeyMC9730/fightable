
//          Sergei Baigerov 2024 - 2026.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <fraylib.h>

struct ftilemap;

#ifdef __cplusplus
extern "C" {
#endif

unsigned char _fLoaderClose();
unsigned char _fLoaderDraw();

void _fLoaderMainPrepareRng();
void _fLoaderMainPrepareStorage();
void _fLoaderMainPrepareNetworking();
void _fLoaderMainPrepareLogging();
void _fLoaderMainPrepareAudio();
int _fLoaderMainPrepareValidFps();
void _fLoaderMainPrepareWindow();
Vector2 _fLoaderMainPrepareUi(Vector2 *win_sz, Vector2 *actual_sz, Vector2 *editor_sz);
void _fLoaderMainProcessAssets(struct ftilemap *tilemapA, struct ftilemap *tilemapB);
void _fLoaderMainProcessArguments(Vector2 *win_sz, Vector2 *actual_sz, Vector2 *editor_sz, unsigned int argc, char **argv, Vector2 *ui_scaling);
void _fLoaderMainTweakUiSettings(Vector2 *actual_sz, Vector2 *editor_sz);
void _fLoaderMainPrepareEnvironment(Vector2 *actual_sz, Vector2 *win_sz, Vector2 *ui_scaling);
void _fLoaderMainLoadAssets();

#ifdef __cplusplus
}
#endif
