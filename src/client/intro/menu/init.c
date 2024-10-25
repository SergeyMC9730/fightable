#include <fightable/intro.h>
#include <raylib.h>
#include <fightable/state.h>
#include <fightable/tilemap.h>

void _fIntroMenuInit() {
    __state.intro_temp_bg = _fTilemapExportTile(*__state.tilemap, (IVector2){28.f, 5.f});
    SetTextureWrap(__state.intro_temp_bg, TEXTURE_WRAP_REPEAT);
}