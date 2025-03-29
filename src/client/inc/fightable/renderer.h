
//          Sergei Baigerov 2024 - 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <fraylib.h>

#ifdef __cplusplus
extern "C" {
#endif

#include <nt5emul/renderer_event.h>
#include <rsb/rsb_array_gen.h>

struct frenderer_sched {
	renderer_event_t func;
	long double delay;
	int times;
};

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

// switch to the camera mode in stack mode
//
// - stack mode means that cameras can be pushed and pulled out of the stack
// - if there are gonna be more than R2D_STACK_SIZE textures inside this stack, this function would behave like a standard `BeginMode2D` function
//
// made for making some routines easier in implementation
void BeginMode2DStacked(Camera2D cam);

// move from the 2d mode in stack mode
//
// - stack mode means that cameras can be pushed and pulled out of the stack
//
// made for making some routines easier in implementation
void EndMode2DStacked();

// tries to find main framebuffer inside the rendertexture stack
// if it fails, it returns -1
// else it returns index inside the stack
int _ntRendererGetMainIdxInStack();

void _fScheduleOverlayFunc(renderer_event_t on_draw);
void _fSchedulerIterateOverlays();
void _fSchedulerVisit();
// schedules a `func` to be called before draw after `delay` seconds.
// required function will be called `n` times.
// ! timer accuracity depends on framerate !
// ! for more precise stuff use _ntInstallTimer and _ntSetupTimerSync !
void _fSchedule(renderer_event_t func, long double delay, int n);

Vector2 _fGetMousePosPix();
Vector2 _fGetMousePosOverlay();

// check if function has been runned inside the renderer thread
unsigned char _ntRendererInThread();

// Draw a grid centered at (0, y, 0)
void DrawGridEx(int slices, float spacing, float y);

#ifdef COTARGET_PTX
void _fPtxInit();
#endif

extern unsigned int UI_SCALE;

#ifdef __cplusplus

}

#include <functional>

using foverlay_callback = std::function<void(Vector2)>;

void _fScheduleOverlayFunc(const foverlay_callback &callback);
#endif
