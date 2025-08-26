
//          Sergei Baigerov 2024 - 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

void _fIntroInit();
void _fIntroDraw();

void _fIntroProcessEndlessDream();
void _fIntroProcessElectricDrug();
void _fIntroProcessCelestialFantasia();

const char *_fIntroGetAuthorName();

void _fIntroMenuInit();
void _fIntroMenuDraw();

int _fIntroGetSeekableRow();

void _fIntroGfxInit();
void _fIntroProcessGfx();
void _fIntroGfxEnable();
void _fIntroGfxDisable();

void _fIntroMenuOnPlay();
void _fIntroMenuOnEditor();
void _fIntroMenuOnOptions();

void _fIntroMenuOnMpCreate();
void _fIntroMenuOnMpJoin();

void _fIntroMenuInitMpBackground();

void _fIntroMenuProcessPlay();
void _fIntroMenuProcessBase();
void _fIntroMenuProcessOptions();
void _fIntroMenuProcessMultiplayer();
void _fIntroMenuProcessMultiplayerJoin();

void _fIntroMenuDrawBackground();

#define INTRO_MENU_BASE_SELECTOR    0
#define INTRO_MENU_PLAY             1
#define INTRO_MENU_MULTIPLAYER      2
#define INTRO_MENU_OPTIONS          3
#define INTRO_MENU_MULTIPLAYER_JOIN 4

#ifdef __cplusplus
}
#endif
