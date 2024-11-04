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

void _fIntroMenuOnMultiplayer();

void _fIntroMenuProcessPlay();
void _fIntroMenuProcessBase();

void _fIntroMenuProcessMultiplayer();

void _fIntroMenuDrawBackground();

#define INTRO_MENU_BASE_SELECTOR    0
#define INTRO_MENU_PLAY             1
#define INTRO_MENU_MULTIPLAYER      2

#ifdef __cplusplus
}
#endif