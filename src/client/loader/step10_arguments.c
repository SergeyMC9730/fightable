
//          Sergei Baigerov 2024 - 2026.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include <fraylib.h>
#include <fightable/state.h>
#include <fightable/renderer.h>
#include <args.h>
#include <nt5emul/tui/environment.h>
#include <fightable/level.h>
#include <fightable/editor.h>

void _fLoaderMainInitHelp(ArgParser *parser) {
    ap_set_helptext(parser, "[LOADER HELP] --ui - set UI mode. available options: \"pc\"; \"android\"\n[LOADER HELP] --session - set preloaded scene. available options: \"main\"; \"editor\"\n[LOADER HELP] --level - set preloaded level. available options: \"none\"; \"[PATH TO THE LEVEL]\"\n[LOADER HELP] --help - print this manual.");
}

void _fLoaderMainProcessArguments(Vector2 *win_sz, Vector2 *actual_sz, Vector2 *editor_sz, unsigned int argc, char **argv, Vector2 *ui_scaling) {
    TraceLog(LOG_INFO, "[LOADER] Looking through any command-line arguments");

    ArgParser* parser = ap_new_parser();
    ap_set_version(parser, "alpha 1.0.0");

    _fLoaderMainInitHelp(parser);

    ap_add_str_opt(parser, "ui", "pc");
    ap_add_str_opt(parser, "session", "main");
    ap_add_str_opt(parser, "level", "none");

    TraceLog(LOG_INFO, "[LOADER] Initialized command-line argument set");

    ap_parse(parser, argc, argv);

    TraceLog(LOG_INFO, "[LOADER] Parsed command-line argument set");

    char *tmp = ap_get_str_value(parser, "level");
    if (strcmp(tmp, "none") != 0) {
        TraceLog(LOG_INFO, "[LOADER] Decision of argument set: loading level from input string \"--level\"");
        __state.current_level = _fLevelLoadFromFile(tmp);
    }
    tmp = ap_get_str_value(parser, "ui");
    if (strcmp(tmp, "android") == 0) {
        TraceLog(LOG_INFO, "[LOADER] Decision of argument set: preparing Android environment");

        UI_SCALE = 5;

        *editor_sz = (Vector2){ 255 / (5.f / (float)UI_SCALE), 0 };
        editor_sz->x *= ui_scaling->x;
        editor_sz->y *= ui_scaling->y;

        _ntTuiLoadEnvironmentDefault(1.25f);

        __state.ui_mode = PU_MOBILE;
    } else {
        _ntTuiLoadEnvironmentDefault(1.f);
    }
    tmp = ap_get_str_value(parser, "session");
    if (strcmp(tmp, "editor") == 0) {
        TraceLog(LOG_INFO, "[LOADER] Decision of argument set: loading level editor first");
        __state.current_editor = _fEditorCreateExt(0);

        actual_sz->x += editor_sz->x; win_sz->x += editor_sz->x;
        actual_sz->y += editor_sz->y; win_sz->y += editor_sz->y;

#ifndef TARGET_ANDROID
        SetWindowSize(actual_sz->x, actual_sz->y);
#endif

        __state.intro_can_continue = 1;

        // Vector2 dpi = GetWindowScaleDPI();

        // UnloadRenderTexture(__state.framebuffer);
        // UnloadRenderTexture(__state.overlay_framebuffer);

        // __state.framebuffer = LoadRenderTexture(__state.initial_game_size.x / UI_SCALE / dpi.x, __state.initial_game_size.y / UI_SCALE / dpi.y);
        // __state.overlay_framebuffer = LoadRenderTexture(__state.initial_game_size.x / dpi.x, __state.initial_game_size.y / dpi.y);
    }

    ap_free(parser);

    TraceLog(LOG_INFO, "[LOADER] No arguments left for processing");
}
