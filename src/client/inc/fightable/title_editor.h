#pragma once

#ifdef __cplusplus
extern "C" {
#endif

struct ftitle_editor;

struct ftitle_editor *_fTitleEditorCreate();
void _fTitleEditorDraw(struct ftitle_editor *editor);

#ifdef __cplusplus
}
#endif
