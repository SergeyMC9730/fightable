#include <fightable/editor.h>
#include <fightable/editor.hpp>
#include <fightable/block.h>
#include <stdio.h>

void _fEditorPlaceBlock(struct feditor *editor, unsigned short id, IVector2 pos) {
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

        return;
    }

    block.layer_id = layer_id;
    editor->objects[layer_id][pos.x][pos.y] = block;
}