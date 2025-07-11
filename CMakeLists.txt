
#          Sergei Baigerov 2024 - 2025.
# Distributed under the Boost Software License, Version 1.0.
#    (See accompanying file LICENSE.txt or copy at
#          https://www.boost.org/LICENSE_1_0.txt)

cmake_minimum_required(VERSION 3.26)
project(fightable LANGUAGES C CXX)

get_cmake_property(CACHE_VARS CACHE_VARIABLES)
foreach(CACHE_VAR ${CACHE_VARS})
    get_property(CACHE_VAR_HELPSTRING CACHE ${CACHE_VAR} PROPERTY HELPSTRING)
    if(CACHE_VAR_HELPSTRING STREQUAL "No help, variable specified on the command line.")
        get_property(CACHE_VAR_TYPE CACHE ${CACHE_VAR} PROPERTY TYPE)
        if(CACHE_VAR_TYPE STREQUAL "UNINITIALIZED")
            set(CACHE_VAR_TYPE)
        else()
            set(CACHE_VAR_TYPE :${CACHE_VAR_TYPE})
        endif()
        set(CMAKE_ARGS "${CMAKE_ARGS} -D${CACHE_VAR}${CACHE_VAR_TYPE}=\"${${CACHE_VAR}}\"")
    endif()
endforeach()
message("CMAKE_ARGS: ${CMAKE_ARGS}")

set(CMAKE_EXPORT_COMPILE_COMMANDS On)
set(FIGHTABLE_COMPILER ${CMAKE_CXX_COMPILER_ID})
set(CMAKE_POLICY_VERSION_MINIMUM 3.5)

include(ext/raylib/cmake/EnumOption.cmake)

enum_option(FIGHTABLE_RENDERER "raylib;frasterizer" "Renderer used for Fightable")

if (NOT DEFINED FIGHTABLE_RENDERER)
    set(FIGHTABLE_RENDERER raylib)
endif()

add_compile_definitions(TARGET_RDR="${FIGHTABLE_RENDERER}")
set(FIGHTABLE_RENDERER_INC src/rfrontend/inc)

if (FIGHTABLE_RENDERER STREQUAL "raylib")
    set(FIGHTABLE_RENDERER_LIB raylib)
    set(FIGHTABLE_RENDERER_INC ${FIGHTABLE_RENDERER_INC} ext/raylib/src)
    add_compile_definitions(TARGET_RDR_ID=0)
endif()
if (FIGHTABLE_RENDERER STREQUAL "frasterizer")
    set(FIGHTABLE_RENDERER_LIB frfrontend)
    set(FIGHTABLE_RENDERER_INC ${FIGHTABLE_RENDERER_INC})
    add_compile_definitions(TARGET_RDR_ID=1)

    include(FindX11)

    add_library(frfrontend STATIC
        src/rfrontend/frasterizer/window.c
        src/rfrontend/frasterizer/state.c
        src/rfrontend/frasterizer/raylib_char.c
        src/rfrontend/frasterizer/draw.c
    )

    if (X11_FOUND)
        add_compile_definitions(TARGET_SUPPORTS_X11)
        target_link_libraries(frfrontend ${X11_LIBRARIES})
        set(FIGHTABLE_RENDERER_INC ${FIGHTABLE_RENDERER_INC} ${X11_INCLUDE_DIR})
    endif()

    target_include_directories(frfrontend PRIVATE
        ${FIGHTABLE_RENDERER_INC}
    )
endif()

set(FIGHTABLE_SHARED_INC
    src/shared/inc
)

add_library(md5-c STATIC
    ext/md5-c/md5.c
)
target_include_directories(md5-c PRIVATE
    ext/md5-c
)

add_library(fshared STATIC
    src/shared/src/cstr.c
    src/shared/src/uchar.c
    src/shared/src/auchar.c
    src/shared/src/time.c
    src/shared/src/string.c
    src/shared/src/char.c
    src/shared/src/pchar.c
    src/shared/src/tools.cpp
)
target_include_directories(fshared PRIVATE
    ${FIGHTABLE_SHARED_INC}
)

if (LIBC_FD_SIZE)
    message(STATUS "Setting MP stack definition, FD_SETSIZE, to ${LIBC_FD_SIZE}")
    add_compile_definitions(FD_SETSIZE=${LIBC_FD_SIZE})
endif()

if (FIGHTABLE_IMGUI)
    add_compile_definitions(_FIGHTABLE_IMGUI_)
endif()

if (OPENMPT_LIB)
    set(OPENMPT_LIB_CUSTOM TRUE)
endif()
if (MICROHTTPD_LIB)
    set(MICROHTTPD_LIB_CUSTOM TRUE)
endif()

include(CheckIncludeFile)

if (NOT OVERRIDE_CXX_STD)
    set(OVERRIDE_CXX_STD 20)
endif()

find_library(OPENMPT_LIB openmpt)
if (NOT OPENMPT_LIB)
    message(FATAL_ERROR "libopenmpt could not be found")
endif()

message(STATUS "libopenmpt lib path: ${OPENMPT_LIB}")

CHECK_INCLUDE_FILE("libopenmpt/libopenmpt.h" HAVE_LIBOPENMPT)
if (NOT HAVE_LIBOPENMPT AND NOT OPENMPT_INC)
    message(FATAL_ERROR "libopenmpt binaries were found but the header files itself are missing")
endif()
message(STATUS "libopenmpt custom include path: ${OPENMPT_INC}")

if (WIN32)
    find_library(WIN32_WS_LIB ws2)
    if (NOT WIN32_WS_LIB)
        message(WARNING "Ws2_32 could not be found")
        set(DISABLE_MP_FUNCTIONALITY ON)
    endif()
endif()

if (DISABLE_MP_FUNCTIONALITY)
    set(DISABLE_MP_SERVER ON)
    add_compile_definitions(FIGHTABLE_NO_MULTIPLAYER)
endif()

if (NOT DISABLE_MP_SERVER)
    find_library(MICROHTTPD_LIB microhttpd)
    if(NOT MICROHTTPD_LIB)
        message(FATAL_ERROR "libmicrohttpd could not be found")
    endif()

    message(STATUS "libmicrohttpd lib path: ${MICROHTTPD_LIB}")

    CHECK_INCLUDE_FILE("microhttpd.h" HAVE_LIBMICROHTTPD)
    if (NOT HAVE_LIBMICROHTTPD AND NOT MICROHTTPD_INC)
        message(FATAL_ERROR "libmicrohttpd binaries were found but the header files itself are missing")
    endif()
    message(STATUS "libmicrohttpd custom include path: ${MICROHTTPD_INC}")

    if (WIN32)
        set(FIGHTABLE_NATIVE_LIBS ${FIGHTABLE_NATIVE_LIBS} Ws2_32)
    endif()
else()
    set(OVERRIDE_CXX_STD 17)
endif()

set(CMAKE_CXX_STANDARD ${OVERRIDE_CXX_STD})
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
set(CXX_STANDARD ${OVERRIDE_CXX_STD})
set(CXX_STANDARD_REQUIRED TRUE)
set(BUILD_SHARED_LIBS ON)
option(JACK OFF)
set(JACK_FOUND OFF)

if (ANDROID)
    set(PLATFORM Android)

    message(STATUS "Building Fightable for Android. Main executable would be built as library instead")
endif()

if (NOT DISABLE_PORTAUDIO)
    set(WDMKS OFF)
    add_subdirectory(ext/portaudio_opensles)
else()
    add_compile_definitions(_DISABLE_PORTAUDIO_)
endif()

set(BUILD_SHARED_LIBS OFF)
set(ENABLE_CJSON_UNINSTALL OFF)
set(ENABLE_CJSON_TEST OFF)
set(ENABLE_TARGET_EXPORT OFF)

if (FIGHTABLE_COMPILER STREQUAL "Clang" AND WIN32)
    set(ENABLE_CUSTOM_COMPILER_FLAGS OFF)
endif()

add_subdirectory(ext/cJSON)
if (FIGHTABLE_RENDERER STREQUAL "raylib")
    add_subdirectory(ext/raylib)
endif()
if (NOT ANDROID)
    add_subdirectory(ext/nativefiledialog-extended)
endif()

set(FIGHTABLE_SRC
    src/client/main.c
    src/client/etc/state.c
    src/client/renderer/ui/tilemap.c
    src/client/etc/intvec.c
    src/client/renderer/draw.c
    src/client/level/draw.c
    src/client/level/load_test.c
    src/client/camera/load_default.c
    src/client/editor/load.cpp
    src/client/editor/draw.cpp
    src/client/block/id.c
    src/client/renderer/ui/text.c
    src/client/renderer/ui/square_button.c
    src/client/hitbox/clip_x.c
    src/client/hitbox/clip_y.c
    src/client/hitbox/expand.c
    src/client/hitbox/intersects.c
    src/client/hitbox/move.c
    src/client/level/get_hitboxes.c
    src/client/editor/contains_id.cpp
    src/client/editor/first_id.cpp
    src/client/renderer/ui/button.c
    src/client/entity/base/update.c
    src/client/intro/init.c
    src/client/entity/base/draw.c
    src/client/level/find_player.c
    src/client/anim/anim_id.c
    src/client/anim/animation.c
    src/client/anim/parse_animation.c
    src/client/anim/parse_keyframe.c
    src/client/anim/print.c
    src/client/anim/set.c
    src/client/intro/draw.c
    src/client/audio/begin.c
    src/client/audio/play_module.c
    src/client/audio/stop.c
    src/client/audio/loop.c
    src/client/audio/get_playtime.c
    src/client/intro/menu/init.c
    src/client/intro/menu/draw.c
    src/client/renderer/gfx/shake.c
    src/client/renderer/gfx/update.c
    src/client/audio/debug.c
    src/client/renderer/begin_texture_mode.c
    src/client/renderer/end_texture_mode.c
    src/client/renderer/stack_find.c
    src/client/audio/channels_total.c
    src/client/renderer/ui/rect.c
    src/client/editor/swipe_cur_objects.cpp
    src/client/editor/in_playback_mode.cpp
    src/client/audio/get_song_name.c
    src/client/intro/electric_drug.c
    src/client/intro/endless_dream.c
    src/client/intro/seekable_row.c
    src/client/etc/storage.c
    src/client/intro/menu/on_play.c
    src/client/intro/menu/on_options.c
    src/client/intro/menu/on_editor.c
    src/client/intro/menu/process_play.c
    src/client/intro/menu/process_options.c
    src/client/intro/menu/draw_background.c
    src/client/intro/menu/process_base.c
    src/client/renderer/gfx/draw.c
    src/client/renderer/gfx/fade.c
    src/client/etc/color.c
    src/client/entity/base/init.c
    src/client/entity/base/jump.c
    src/client/entity/base/move.c
    src/client/entity/lplayer/init.c
    src/client/entity/lplayer/update.c
    src/client/entity/lplayer/draw.c
    src/client/entity/base/draw_pos.c
    src/client/entity/base/setters.c
    src/client/intro/gfx.cpp
    src/client/renderer/keyboard.c
    src/client/renderer/overlay_scheduler.cpp
    src/client/renderer/mouse.c
    src/client/editor/place_block.cpp
    src/client/audio/volume.c
    src/client/etc/sanitizer.c
    src/client/intro/menu/on_mp_create.c
    src/client/intro/menu/on_mp_join.c
    src/client/renderer/ui/slider.c
    src/client/entity/base/accessory.c
    src/client/entity/base/cleanup.c
    src/client/etc/config.c
    src/client/entity/base/damage.c
    src/client/etc/mixer.c
    src/client/entity/lplayer/damage.c
    src/client/block/update.c
    src/client/audio/fx.c
    src/client/entity/lplayer/perform_death.c
    src/client/level/gameover.c
    src/client/renderer/gfx/damage_overlay.c
    src/client/entity/text.c
    src/client/level/destroy_entity.c
    src/client/level/light_source.c
    src/client/mpserver/draw.c
    src/client/mpserver/delegate.c
    src/client/level/draw_pixelated.c
    src/client/level/save.cpp
    src/client/level/load.c
    src/client/etc/unitype.c
    src/client/renderer/grid_3d.c
    src/client/block/bitfield.c
    src/client/level/destroy.c
    src/client/level/block_processor.c
    src/client/entity/base/getters.c
    src/client/intro/celestial_fantasia.c
    src/client/camera/mode2d.c
    src/client/ntldr_tui/fileselector_click.c
    src/client/ntldr_tui/fileselector_load.c
    src/client/ntldr_tui/fileselector_setlisting.c
    src/client/ntldr_tui/fileselector_unload.c
    src/client/ntldr_tui/fileselector_update.c
    src/client/ntldr_tui/frame.c
    src/client/ntldr_tui/get_environment.c
    src/client/ntldr_tui/input.c
    src/client/ntldr_tui/load_environment.c
    src/client/ntldr_tui/menu.c
    src/client/ntldr_tui/progress_bar.c
    src/client/ntldr_tui/rectangle.c
    src/client/ntldr_tui/state.c
    src/client/ntldr_tui/text.c
    src/client/etc/rsb_color.c
    src/client/renderer/file_selector.c
    src/client/renderer/ui/notification.c
    src/client/etc/notif_mgr.c
    src/client/etc/pthread_compat.c
    ${FIGHTABLE_GPU_SRC}
)

set(FIGHTABLE_TCP_SERVER_SRC
    src/tcpsrv/src/daemon.cpp
    src/tcpsrv/src/message.cpp
    src/tcpsrv/src/user.cpp
)
set(FIGHTABLE_TCP_CLIENT_SRC
    "src/tcpclient/src/create.cpp"
    src/tcpclient/src/thread.cpp
    "src/tcpclient/src/destroy.cpp"
    "src/tcpclient/src/send_msg.cpp"
    src/tcpclient/src/disconnect.c
)

if (ANDROID)
    message(STATUS "Compiling for Android")
    set(FIGHTABLE_TARGET Android)

    set(FIGHTABLE_SRC
        ${FIGHTABLE_SRC}
    )

    include_directories($ENV{ANDROID_NDK}/sources/android/native_app_glue)

    add_library(fbase STATIC ${FIGHTABLE_SRC})
    add_library(fightable SHARED src/launcher/main.c $ENV{ANDROID_NDK}/sources/android/native_app_glue/android_native_app_glue.c)

    set(FIGHTABLE_NATIVE_LIBS ${FIGHTABLE_NATIVE_LIBS} android log m)

    add_compile_definitions(TARGET_ANDROID)
    add_compile_definitions(TARGET_UNIX)
    add_compile_definitions(TARGET_LINUX)
elseif(WIN32)
    message(STATUS "Compiling for NT-like operating systems")
    set(FIGHTABLE_TARGET NT)

    set(BUILD_SHARED_LIBS OFF)
    add_subdirectory(ext/dirent)
    add_subdirectory(ext/pthread-win32)

    add_library(fbase STATIC ${FIGHTABLE_SRC})
    add_executable(fightable src/launcher/main.c)
    add_compile_definitions(TARGET_WIN32)
    add_compile_definitions(__MINGW32__)
    add_compile_definitions(__MINGW64__)
    add_compile_definitions(WINVER=0x601)
    add_compile_definitions(_WIN32_WINNT=0x601)
    # add_compile_definitions(_M_X64=100)

    set(FIGHTABLE_NATIVE_LIBS ${FIGHTABLE_NATIVE_LIBS} pthreadVC3 dirent)
    set(FIGHTABLE_NATIVE_INCLUDES ${FIGHTABLE_NATIVE_INCLUDES} ext/pthread-win32 ext/strings_h ext/dirent/include)

    if (FIGHTABLE_COMPILER STREQUAL "Clang")
        set(FIGHTABLE_COPTS "-Wreserved-macro-identifier -Wunsafe-buffer-usage")
        message(STATUS "Applying Clang patches for NT target")
    elseif (FIGHTABLE_COMPILER STREQUAL "MSVC")
        if (NOT FIGHTABLE_ALLOW_MSVC)
            message(FATAL_ERROR "Fightable cannot be built using MSVC. Use -DFIGHTABLE_ALLOW_MSVC=1 to continue configuring")
        else()
            message(STATUS "Allowing MSVC to build this source. Expect issues to come out after configuring")
        endif()
    endif()
elseif(LINUX)
    message(STATUS "Compiling for GNU/Linux")

    set(FIGHTABLE_TARGET Linux)

    add_library(fbase STATIC ${FIGHTABLE_SRC})
    add_executable(fightable src/launcher/main.c)
    add_compile_definitions(TARGET_UNIX)
    add_compile_definitions(TARGET_LINUX)

    set(FIGHTABLE_NATIVE_LIBS ${FIGHTABLE_NATIVE_LIBS} pthread m)
elseif(UNIX)
    message(STATUS "Compiling for Unix-like operating systems with X Window System")

    set(FIGHTABLE_TARGET Unix)

    add_library(fbase STATIC ${FIGHTABLE_SRC})
    add_executable(fightable src/launcher/main.c)
    add_compile_definitions(TARGET_UNIX)
    set(FIGHTABLE_NATIVE_LIBS ${FIGHTABLE_NATIVE_LIBS} pthread m)
else()
    message(STATUS "Compiling for unknown platform")
    set(FIGHTABLE_TARGET Unknown platform)
    add_compile_definitions(TARGET_UNKNOWN_PLATFORM)

    message(FATAL_ERROR "Fightable does not support targets beyond Unix(X11) and NT")
endif()

if (UNIX OR LINUX)
    message(STATUS "Applying linkage patches for Unix")

    target_sources(fshared PRIVATE src/shared/src/assert.c)
    add_subdirectory(ext/safestringlib)

    set(FIGHTABLE_NATIVE_LIBS ${FIGHTABLE_NATIVE_LIBS} safestring_static)
endif()

include (TestBigEndian)
TEST_BIG_ENDIAN(IS_BIG_ENDIAN)
if (IS_BIG_ENDIAN)
    message(STATUS "Target CPU is big-endian")
    add_compile_definitions(TARGET_BIG_ENDIAN)
else ()
    message(STATUS "Target CPU is little-endian")
endif ()

set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} ${FIGHTABLE_COPTS}")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} ${FIGHTABLE_COPTS}")
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS_DEBUG} ${FIGHTABLE_COPTS}")
# add_compile_options(${FIGHTABLE_COPTS})

if (NOT DISABLE_MP_SERVER)
    add_library(ftcpsrv STATIC ${FIGHTABLE_TCP_SERVER_SRC})
    target_link_libraries(ftcpsrv ${FIGHTABLE_NATIVE_LIBS} fshared md5-c)
    target_include_directories(ftcpsrv PRIVATE
        ${FIGHTABLE_SHARED_INC}
        ${FIGHTABLE_NATIVE_INCLUDES}
        ext/md5-c
    )

    add_executable(ftcpsrv-test src/tcpsrv_test/main.c)
    target_link_libraries(ftcpsrv-test ftcpsrv ${FIGHTABLE_NATIVE_LIBS} fshared)
    target_include_directories(ftcpsrv-test PRIVATE
        src/tcpsrv/inc
        ${FIGHTABLE_SHARED_INC}
        ${FIGHTABLE_NATIVE_INCLUDES}
    )

    add_executable(tcpserver-raylib-test src/tcpserver_raylib/main.c)
    target_link_libraries(tcpserver-raylib-test ftcpsrv ${FIGHTABLE_NATIVE_LIBS} fshared)
    target_include_directories(tcpserver-raylib-test PRIVATE
        src/tcpsrv/inc
        ${FIGHTABLE_SHARED_INC}
        ${FIGHTABLE_NATIVE_INCLUDES}
        ${FIGHTABLE_RENDERER_INC}
    )
endif()

if (NOT DISABLE_MP_FUNCTIONALITY)
    add_library(ftcpclient STATIC ${FIGHTABLE_TCP_CLIENT_SRC})
    target_link_libraries(ftcpclient fshared ${FIGHTABLE_NATIVE_LIBS} md5-c)
    target_include_directories(ftcpclient PRIVATE
        src/tcpclient/inc
        ${FIGHTABLE_SHARED_INC}
        ${FIGHTABLE_NATIVE_INCLUDES}
        ext/md5-c
    )

    add_executable(ftcpclient-test src/tcpclient_test/main.c)
    target_link_libraries(ftcpclient-test ftcpclient ${FIGHTABLE_NATIVE_LIBS} md5-c)
    target_include_directories(ftcpclient-test PRIVATE
        src/tcpclient/inc
        ${FIGHTABLE_SHARED_INC}
        ${FIGHTABLE_NATIVE_INCLUDES}
    )

    add_executable(tcpclient-raylib-test src/tcpclient_raylib/main.c)
    target_link_libraries(tcpclient-raylib-test ${FIGHTABLE_RENDERER_LIB} ftcpclient ${FIGHTABLE_NATIVE_LIBS})
    target_include_directories(tcpclient-raylib-test PRIVATE
        src/tcpclient/inc
        ${FIGHTABLE_RENDERER_INC}
        ${FIGHTABLE_SHARED_INC}
        ${FIGHTABLE_NATIVE_INCLUDES}
    )
endif()

add_executable(fightable-simple-window src/simple_window/src/main.c)
target_link_libraries(fightable-simple-window ${FIGHTABLE_RENDERER_LIB} ${FIGHTABLE_NATIVE_LIBS})
target_include_directories(fightable-simple-window PRIVATE
    ${FIGHTABLE_RENDERER_INC}
    ${FIGHTABLE_SHARED_INC}
    ${FIGHTABLE_NATIVE_INCLUDES}
)

add_executable(anim_editor
    src/anim_editor/main.cpp
    src/anim_editor/tab.cpp
    src/anim_editor/tab_keyframe_prop.cpp
    src/anim_editor/tab_keyframes.cpp
    src/anim_editor/tab_viewpoint.cpp
    src/anim_editor/state.cpp
)
target_link_libraries(anim_editor fbase ${FIGHTABLE_RENDERER_LIB} ${FIGHTABLE_NATIVE_LIBS})
target_include_directories(anim_editor PRIVATE
    src/client/inc
    src/anim_editor/inc
    ${FIGHTABLE_RENDERER_INC}
    ${FIGHTABLE_SHARED_INC}
    ${FIGHTABLE_NATIVE_INCLUDES}
)

if (NOT DISABLE_MP_SERVER)
    if (WIN32)
        set(HTTPSERVER_NAME libhttpserver-win32)
    else()
        set(HTTPSERVER_NAME libhttpserver)
    endif()

    file(GLOB HTTPSERVER_SRC
        "ext/${HTTPSERVER_NAME}/src/*.hpp"
        "ext/${HTTPSERVER_NAME}/src/*.h"
        "ext/${HTTPSERVER_NAME}/src/*.cpp"

        "ext/${HTTPSERVER_NAME}/src/details/*.cpp"

        "ext/${HTTPSERVER_NAME}/src/httpserver/*.hpp"
        "ext/${HTTPSERVER_NAME}/src/httpserver/details/*.hpp"
    )

    set(HTTPSERVER_INC
        ext/${HTTPSERVER_NAME}/src
        ext/${HTTPSERVER_NAME}/src/details
        ext/${HTTPSERVER_NAME}/src/httpserver
        ext/${HTTPSERVER_NAME}/src/httpserver/details
        ${FIGHTABLE_NATIVE_INCLUDES}
    )

    add_library(httpserver STATIC ${HTTPSERVER_SRC})
    target_include_directories(httpserver PRIVATE
        ${HTTPSERVER_INC}
        ${MICROHTTPD_INC}
        ${FIGHTABLE_SHARED_INC}
    )
    add_compile_definitions(_HTTPSERVER_HPP_INSIDE_)

    target_link_libraries(httpserver PUBLIC ${MICROHTTPD_LIB})

    add_library(fightable-http STATIC
        src/http/req/APIRequest.cpp
        src/http/req/SplitString.cpp
        src/http/content_type/HttpContentType.cpp
        src/http/content_type/HttpContentTypeAudio.cpp
        src/http/content_type/HttpContentTypeHtml.cpp
        src/http/content_type/HttpContentTypeImage.cpp
        src/http/content_type/HttpContentTypeJson.cpp
        src/http/content_type/HttpContentTypeText.cpp
        src/http/req/v1.helloworld.cpp
        src/http/http_server_create.cpp
        src/http/http_server_destroy.cpp
        src/http/req/v1.res.request.cpp
        src/http/resource_dir.cpp
        src/http/content_type/HttpContentTypeOctet.cpp
    )
    target_include_directories(fightable-http PRIVATE
        src/http/inc
        ${HTTPSERVER_INC}
        ${MICROHTTPD_INC}
        ${FIGHTABLE_SHARED_INC}
    )
    target_link_libraries(fightable-http PUBLIC httpserver)
else()
    add_compile_definitions(_DISABLE_MP_SERVER_)
endif()

if (NOT DISABLE_MP_SERVER)
    target_include_directories(fightable-http PRIVATE ${FIGHTABLE_NATIVE_INCLUDES} ${FIGHTABLE_SHARED_INC})
    set(FIGHTABLE_HTTP_LINK fightable-http)
    set(FIGHTABLE_HTTP_INC src/http/inc)

    target_include_directories(ftcpsrv PRIVATE
        src/tcpsrv/inc
        ext/base64/include
        ${FIGHTABLE_NATIVE_INCLUDES}
        ${FIGHTABLE_SHARED_INC}
    )

    add_executable(mp-rect-test
        src/mp_rect_test/src/main.cpp
    )
    target_include_directories(mp-rect-test PRIVATE
        ${FIGHTABLE_RENDERER_INC}
        ${FIGHTABLE_SHARED_INC}
        ${FIGHTABLE_NATIVE_INCLUDES}
        src/tcpclient/inc
        src/tcpsrv/inc
    )
    target_link_libraries(mp-rect-test ${FIGHTABLE_RENDERER_LIB} ftcpclient ${FIGHTABLE_NATIVE_LIBS} fshared)
endif()

file(COPY ${CMAKE_SOURCE_DIR}/src/assets DESTINATION ${CMAKE_BINARY_DIR}/)

add_executable(physics-test
    src/physics_test/main.c
)

add_executable(collide-test
    src/collide_test/main.c
    src/collide_test/hitbox/clip_x.c
    src/collide_test/hitbox/clip_y.c
    src/collide_test/hitbox/move.c
)
add_executable(gun-test
    src/shooting_system_test/src/main.cpp
    src/shooting_system_test/src/hitbox/clip_x.c
    src/shooting_system_test/src/hitbox/clip_y.c
    src/shooting_system_test/src/hitbox/expand.c
    src/shooting_system_test/src/hitbox/intersects.c
    src/shooting_system_test/src/hitbox/move.c
)

target_include_directories(fbase PRIVATE
    src/client/inc
    ${FIGHTABLE_RENDERER_INC}
    ext/cJSON
    ext/portaudio_opensles/include
    src/tcpclient/inc
    src/tcpsrv/inc
    ext/nativefiledialog-extended/src/include
    ext/PerlinNoise
    ${FIGHTABLE_HTTP_INC}
    ${OPENMPT_INC}
    ${FIGHTABLE_NATIVE_INCLUDES}
    ${FIGHTABLE_SHARED_INC}
)
target_include_directories(fightable PRIVATE
    src/client/inc
)

target_include_directories(physics-test PRIVATE
    ${FIGHTABLE_RENDERER_INC}
    ${FIGHTABLE_SHARED_INC}
    ${FIGHTABLE_NATIVE_INCLUDES}
)
target_include_directories(gun-test PRIVATE
    ${FIGHTABLE_RENDERER_INC}
    src/shooting_system_test/include
    ${FIGHTABLE_SHARED_INC}
    ${FIGHTABLE_NATIVE_INCLUDES}
)
target_include_directories(collide-test PRIVATE
    ${FIGHTABLE_RENDERER_INC}
    src/collide_test/inc
    ${FIGHTABLE_SHARED_INC}
    ${FIGHTABLE_NATIVE_INCLUDES}
)
if (ANDROID)
    set(NFD_LINK )
else ()
    set(NFD_LINK nfd)
endif ()
if (NOT DISABLE_PORTAUDIO)
    target_link_libraries(fbase ${FIGHTABLE_RENDERER_LIB} cjson PortAudio ${OPENMPT_LIB} ${FIGHTABLE_NATIVE_LIBS} ${FIGHTABLE_HTTP_LINK} ftcpsrv fshared ${NFD_LINK})
else()
    target_link_libraries(fbase ${FIGHTABLE_RENDERER_LIB} cjson ${OPENMPT_LIB} ${FIGHTABLE_NATIVE_LIBS} ${FIGHTABLE_HTTP_LINK} ftcpsrv fshared ${NFD_LINK})
endif()
target_link_libraries(fightable fbase)
target_link_libraries(physics-test ${FIGHTABLE_RENDERER_LIB} )
target_link_libraries(collide-test ${FIGHTABLE_RENDERER_LIB} )
target_link_libraries(gun-test ${FIGHTABLE_RENDERER_LIB} )

if (NOT DISABLE_MP_SERVER)
    target_link_libraries(mp-rect-test ftcpsrv)
endif()

message(STATUS "")
message(STATUS "")

message(STATUS "           Fightable Compile Settings")
if (DISABLE_MP_SERVER)
    message(STATUS "  MP server is disabled")
endif()
message(STATUS "  CXX standard: ${OVERRIDE_CXX_STD}")
message(STATUS "  Platform: ${FIGHTABLE_TARGET}")

if (OPENMPT_INC)
    message(STATUS "  libopenmpt custom include path: ${OPENMPT_INC}")
endif()
if (MICROHTTPD_INC)
    message(STATUS "  libmicrohttpd custom include path: ${MICROHTTPD_INC}")
endif()

if (OPENMPT_LIB_CUSTOM)
    message(STATUS "  libopenmpt lib path: ${OPENMPT_LIB}")
endif()
if (MICROHTTPD_LIB_CUSTOM)
    message(STATUS "  libmicrohttpd lib path: ${MICROHTTPD_LIB}")
endif()

message(STATUS "  Compiler: ${FIGHTABLE_COMPILER}")
message(STATUS "  Build type: ${CMAKE_BUILD_TYPE}")

if (DISABLE_PORTAUDIO)
    message(STATUS "  PortAudio is disabled")
endif()

message(STATUS "  Renderer: ${FIGHTABLE_RENDERER}")

if (DISABLE_MP_FUNCTIONALITY)
    message(STATUS "  MP functionality is disabled")
endif()

message(STATUS "")
message(STATUS "")

#########
#########
