#include <fightable/editor.hpp>
#include <fightable/editor.h>

unsigned char _fEditorInPlaybackMode(struct feditor *editor) {
    return editor->should_playback;
}