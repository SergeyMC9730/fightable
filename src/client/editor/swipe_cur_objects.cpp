#include <fightable/editor.hpp>
#include <fightable/editor.h>

void _fEditorSwipeCurrentObjects(struct feditor *editor, float swipe_delta) {
    TraceLog(LOG_INFO, "Swiping objects by %f", swipe_delta);

    if (swipe_delta > 0.f) {
        editor->current_block_id = (editor->current_block_id + 1) % editor->block_listing.total;
    } else if (swipe_delta < 0.f) {
        if (editor->current_block_id == 0) {
            editor->current_block_id = editor->block_listing.total - 1;
        } else {
            editor->current_block_id = (editor->current_block_id - 1) % editor->block_listing.total;
        }
    }
}