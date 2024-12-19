#include <fightable/intro.h>
#include <fightable/intvec.h>
#include <fightable/state.h>
#include <stdlib.h>
#include <fightable/button.h>

void _fIntroMenuProcessBase() {
    int wx = __state.framebuffer.texture.width;
    int wy = __state.framebuffer.texture.height;

    Color tint = WHITE;

    if (_fButtonDrawSimple("PLAY", (IVector2){(wx - (3 * __state.tilemap->tile_size.x)) / 2 + __state.menu_cur_x, 50}, WHITE)) {
        _fIntroMenuOnPlay();
    }
            
    if (_fButtonDrawSimple("OPTIONS", (IVector2){(wx - (4 * __state.tilemap->tile_size.x)) / 2 + __state.menu_cur_x, 60}, WHITE)) {
        _fIntroMenuOnOptions();
    }

    if (_fButtonDrawSimple("EDITOR", (IVector2){(wx - (4 * __state.tilemap->tile_size.x)) / 2 + __state.menu_cur_x, 70}, WHITE)) {
        _fIntroMenuOnEditor();
    }
    if (_fButtonDrawSimple("EXIT", (IVector2){(wx - (3 * __state.tilemap->tile_size.x)) / 2 + __state.menu_cur_x, 80}, WHITE)) {
        _fConfigSave(&__state.config);
        exit(0);
    }
}