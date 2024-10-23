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

#ifdef __cplusplus
}
#endif