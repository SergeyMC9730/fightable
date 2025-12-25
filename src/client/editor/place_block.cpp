#include "fightable/block_library.h"
#include "fightable/level.h"
#include <fightable/editor.h>
#include <fightable/editor.hpp>
#include <fightable/block.h>
#include <fightable/nbt_tools.h>

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

        struct flevel_registry_entry entry = { 0 };
        entry.id = ++editor->level->last_entry_id;
        entry.entry = nbt_new_tag_compound();
        entry.valid = entry.id == editor->level->last_entry_id && entry.entry != NULL && entry.id != 0;

        b.linked_reg = RSBAddElement_lre(editor->level->block_entries, entry);
        b.registry_id = entry.id;
        editor->objects[layer_id][pos.x][pos.y] = b;

        TraceLog(LOG_INFO, "(1) Block ID: %d", entry.id);

        return;
    }

    block.layer_id = layer_id;

    struct flevel_registry_entry entry = { 0 };
    entry.id = ++editor->level->last_entry_id;

    if (id == BLOCK_TMOVE) {
        entry.entry = nbt_new_tag_compound();
        nbt_tag_t *tag_tox = nbt_new_tag_short(4);
        nbt_tag_t *tag_toy = nbt_new_tag_short(8);

        nbt_set_tag_name_easy(tag_tox, "tmp_offset_x");
        nbt_set_tag_name_easy(tag_toy, "tmp_offset_y");

        nbt_tag_compound_append(entry.entry, tag_tox);
        nbt_tag_compound_append(entry.entry, tag_toy);
    } else {
        entry.entry = nbt_new_tag_compound();
    }
    entry.valid = entry.id == editor->level->last_entry_id && entry.entry != NULL && entry.id != 0;

    block.linked_reg = RSBAddElement_lre(editor->level->block_entries, entry);
    block.registry_id = entry.id;
    editor->objects[layer_id][pos.x][pos.y] = block;

    TraceLog(LOG_INFO, "(2) Block ID: %d", entry.id);
}
