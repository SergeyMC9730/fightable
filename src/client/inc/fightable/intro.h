#pragma once

#ifdef __cplusplus
extern "C" {
#endif

void _fIntroInit();
void _fIntroDraw();

void _fIntroProcessEndlessDream();
void _fIntroProcessElectricDrug();

void _fIntroMenuInit();
void _fIntroMenuDraw();

int _fIntroGetSeekableRow();

void _fIntroGfxInit();
void _fIntroProcessGfx();

void _fIntroMenuOnPlay();
void _fIntroMenuOnEditor();
void _fIntroMenuOnOptions();

void _fIntroMenuOnMpCreate();
void _fIntroMenuOnMpJoin();

void _fIntroMenuProcessPlay();
void _fIntroMenuProcessBase();
void _fIntroMenuProcessOptions();

void _fIntroMenuProcessMultiplayer();

void _fIntroMenuDrawBackground();

#define INTRO_MENU_BASE_SELECTOR    0
#define INTRO_MENU_PLAY             1
#define INTRO_MENU_MULTIPLAYER      2
#define INTRO_MENU_OPTIONS          3

#ifdef __cplusplus
}
#endif