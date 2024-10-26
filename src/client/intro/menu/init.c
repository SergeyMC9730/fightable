#include <fightable/intro.h>
#include <raylib.h>
#include <fightable/state.h>
#include <fightable/tilemap.h>

void _fIntroMenuInit() {
    __state.intro_temp_bg = _fTilemapExportTile(*__state.tilemap, (IVector2){28.f, 5.f});
    SetTextureWrap(__state.intro_temp_bg, TEXTURE_WRAP_REPEAT);

    __state.test_label = _fTextRenderGradientV(&__state.text_manager, "i wrote gradient stuff!!\ni wrote a newline here.", RED, YELLOW, 1);
}