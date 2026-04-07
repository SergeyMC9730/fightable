
//          Sergei Baigerov 2024 - 2026.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <fightable/intvec.h>

struct feditor;

void _fEditorDraw(struct feditor *editor);
struct feditor *_fEditorCreate();
struct feditor *_fEditorCreateExt(unsigned char with_file_dialog);
unsigned char _fEditorContainsId(struct feditor *editor, unsigned short id);
IVector2 _fEditorGetPosOfFirstId(struct feditor *editor, unsigned short id);
IVector2 _fEditorGetPosOfFirstIdLvl(struct flevel *level, unsigned short id);
void _fEditorSwipeCurrentObjects(struct feditor *editor, float swipe_delta);
unsigned char _fEditorInPlaybackMode(struct feditor *editor);
void _fEditorPlaceBlock(struct feditor *editor, unsigned short id, IVector2 pos);

#ifdef __cplusplus
}
#endif
