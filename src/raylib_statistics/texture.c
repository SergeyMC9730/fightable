//#define _GNU_SOURCE

#include <stdio.h>
#include <unistd.h>
#include <dlfcn.h>
#include <raylib.h>
#include <rlstats/state.h>

void DrawTexturePro(Texture2D texture, Rectangle source, Rectangle dest, Vector2 origin, float rotation, Color tint) {
    void (*func)(Texture2D texture, Rectangle source, Rectangle dest, Vector2 origin, float rotation, Color tint)
    = dlsym(RTLD_NEXT, "DrawTexturePro");

    printf("a\n");
    __state.draw_calls++;

    func(texture, source, dest, origin, rotation, tint);
}


void InitWindow(int w, int h, const char *t) {
    void (*func)(int w, int h, const char *t)
    = dlsym(RTLD_NEXT, "InitWindow");

    printf("--- INIT WINDOW ---\n");
    __state.draw_calls++;

    func(w, h, "hehe");
}