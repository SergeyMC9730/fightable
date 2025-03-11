#include <fightable/editor.h>
#include <fightable/editor.hpp>
#include <fightable/block.h>

void _fEditorPlaceBlock(struct feditor *editor, unsigned short id, IVector2 pos) {
    if (!editor || editor->in_edit_mode) return;

    auto block = _fBlockFromId(id);

    int layer_id = editor->current_layer;
    if (layer_id < 0) layer_id = 0;

    if (block.singular && _fEditorContainsId(editor, id)) {
        IVector2 pos = _fEditorGetPosOfFirstId(editor, id);
        auto air = _fBlockFromId(0);
        air.layer_id = layer_id;

        editor->objects[layer_id][pos.x][pos.y] = air;
    } else if (block.metaobject) {
        std::vector<unsigned short> variants = {id};

        for (int i = 0; i < editor->block_listing.total; i++) {
            fblock b = editor->block_listing.blocks[i];
            if (b.parent_id == id && !b.metaobject) {
                variants.push_back(_fBlockIdFromBlock(b));
            }
        }

        int idx = GetRandomValue(0, variants.size() - 1);
        int _id = variants[idx];

        auto b = _fBlockFromId(_id);
        b.layer_id = layer_id;

        editor->objects[layer_id][pos.x][pos.y] = b;

        struct flevel_registry_entry entry = { 0 };
        entry.id = ++editor->level->last_entry_id;
        entry.entry = __uni_create(NULL);

        RSBAddElement_lre(editor->level->block_entries, entry);

        return;
    }

    block.layer_id = layer_id;
    editor->objects[layer_id][pos.x][pos.y] = block;

    struct flevel_registry_entry entry = { 0 };
    entry.id = ++editor->level->last_entry_id;
    entry.entry = __uni_create(" ");

    RSBAddElement_lre(editor->level->block_entries, entry);
}
