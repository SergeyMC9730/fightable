#pragma once

#include <raylib.h>

#ifdef __cplusplus
extern "C" {
#endif

void _fDraw();

// raylib additions

// switch to the texture mode in stack mode
//
// - stack mode means that rendertextures can be pushed and pulled out of the stack
// - if there are gonna be more than R2D_STACK_SIZE textures inside this stack, this function would behave like a standard `BeginTextureMode` function
//
// made for making some routines easier in implementation
void BeginTextureModeStacked(RenderTexture2D txt);

// move from the texture mode in stack mode
//
// - stack mode means that rendertextures can be pushed and pulled out of the stack
//
// made for making some routines easier in implementation
void EndTextureModeStacked();

// tries to find main framebuffer inside the rendertexture stack
// if it fails, it returns -1
// else it returns index inside the stack
int _ntRendererGetMainIdxInStack();

void _fScheduleOverlayFunc(void (*callback)(void *user), void *user);
void _fSchedulerIterateOverlays();

Vector2 _fGetMousePosPix();
Vector2 _fGetMousePosOverlay();

#ifdef COTARGET_PTX
void _fPtxInit();
#endif

#define UI_SCALE 5

#ifdef __cplusplus

}

#include <functional>

using foverlay_callback = std::function<void(Vector2)>;

void _fScheduleOverlayFunc(const foverlay_callback &callback);
#endif
