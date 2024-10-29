#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <fightable/intvec.h>

struct feditor;

void _fEditorDraw(struct feditor *editor);

struct feditor *_fEditorCreate();

unsigned char _fEditorContainsId(struct feditor *editor, unsigned short id);
IVector2 _fEditorGetPosOfFirstId(struct feditor *editor, unsigned short id);

void _fEditorSwipeCurrentObjects(struct feditor *editor, float swipe_delta);

unsigned char _fEditorInPlaybackMode(struct feditor *editor);

#ifdef __cplusplus
}
#endif