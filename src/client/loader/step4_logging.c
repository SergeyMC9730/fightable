
//          Sergei Baigerov 2024 - 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include <fraylib.h>
#include <fightable/state.h>

#ifdef TARGET_ANDROID

#include <stdarg.h>
#include <android/log.h>
#include <android_native_app_glue.h>

void _fAndroidTraceLog(int level, const char *text, __builtin_va_list args) {
    __android_log_vprint(ANDROID_LOG_VERBOSE, "fightable", text, args);
}
#endif

void _fMainLog(const char *msg) {
    TraceLog(LOG_INFO, msg);
}

void _fLoaderMainPrepareLogging() {
#ifdef TARGET_ANDROID
    SetTraceLogCallback(_fAndroidTraceLog);
    TraceLog(LOG_INFO, "[LOADER] Initialized Android logging");
    __state.show_debug_info = 1;
#endif
    TraceLog(LOG_INFO, "[LOADER] Fightable loader welcomes you");
}
