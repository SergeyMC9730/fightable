cmake_minimum_required(VERSION 3.28)
project(fightable LANGUAGES C CXX)

set(FIGHTABLE_SHARED_INC
    src/shared/inc
)

add_library(fshared STATIC
    src/shared/src/cstr.c
    src/shared/src/uchar.c
    src/shared/src/auchar.c
    src/shared/src/time.c
    src/shared/src/string.c
    src/shared/src/char.c
    src/shared/src/pchar.c
)
target_include_directories(fshared PRIVATE
    ${FIGHTABLE_SHARED_INC}
)

if (FIGHTABLE_CUDA OR DEFINED CMAKE_CUDA_ARCHITECTURES)
    message(STATUS "Enabling PTX co-target")
    if(NOT DEFINED CMAKE_CUDA_ARCHITECTURES)
        set(CMAKE_CUDA_ARCHITECTURES 52)
    endif()
    enable_language(CUDA)

    set(FIGHTABLE_GPU_SRC
        src/client/init_ptx.cu
        src/client/level/prepare_render.cu
        src/client/level/reload_cuda_ctx.cu
    )

    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        message(STATUS "Enabling PTX debugger")
        set(CMAKE_CUDA_FLAGS ${CMAKE_CUDA_FLAGS} "-g -G")
    endif()

    add_compile_definitions(COTARGET_PTX)

    add_executable(cuda-test
        src/cuda_test/gpu.cu
        src/cuda_test/main.c
    )
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
if(NOT OPENMPT_LIB)
    message(FATAL_ERROR "libopenmpt could not be found")
endif()

message(STATUS "libopenmpt lib path: ${OPENMPT_LIB}")

CHECK_INCLUDE_FILE("libopenmpt/libopenmpt.h" HAVE_LIBOPENMPT)
if (NOT HAVE_LIBOPENMPT AND NOT OPENMPT_INC)
	message(FATAL_ERROR "libopenmpt binaries were found but the header files itself are missing")
endif()
message(STATUS "libopenmpt custom include path: ${OPENMPT_INC}")

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
else()
    set(OVERRIDE_CXX_STD 17)
endif()

set(CMAKE_CXX_STANDARD ${OVERRIDE_CXX_STD})
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
set(CXX_STANDARD ${OVERRIDE_CXX_STD})
set(CXX_STANDARD_REQUIRED TRUE)
set(BUILD_SHARED_LIBS ON)

if (ANDROID)
	set(JACK OFF)
	set(JACK_FOUND OFF)
	set(PLATFORM Android)

	message(STATUS "Building Fightable for Android. Main executable would be built as library instead")
endif()

add_subdirectory(ext/portaudio_opensles)

set(BUILD_SHARED_LIBS OFF)
set(ENABLE_CJSON_UNINSTALL OFF)

add_subdirectory(ext/cJSON)
add_subdirectory(ext/raylib)

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
    src/client/serializable/serializable.c
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
    ${FIGHTABLE_GPU_SRC}
)

set(FIGHTABLE_TCP_SERVER_SRC
    src/tcpsrv/src/daemon.cpp
    src/tcpsrv/src/message.cpp
    src/tcpsrv/src/user.cpp
    src/tcpsrv/src/tools.cpp
)
set(FIGHTABLE_TCP_CLIENT_SRC
    src/tcpclient/src/create.c
    src/tcpclient/src/thread.cpp
    src/tcpclient/src/destroy.c
    src/tcpclient/src/send_msg.c
)

if (ANDROID)
    message(STATUS "Compiling for Android")
    set(FIGHTABLE_TARGET Android)

	set(FIGHTABLE_SRC
		${FIGHTABLE_SRC}
		$ENV{ANDROID_NDK}/sources/android/native_app_glue/android_native_app_glue.c
	)

	include_directories($ENV{ANDROID_NDK}/sources/android/native_app_glue)

	add_library(fightable SHARED ${FIGHTABLE_SRC})

	set(FIGHTABLE_NATIVE_LIBS android log)

	add_compile_definitions(TARGET_ANDROID)
    add_compile_definitions(TARGET_UNIX)
elseif(WIN32)
    message(STATUS "Compiling for Windows")
    set(FIGHTABLE_TARGET Windows)

    set(BUILD_SHARED_LIBS OFF)
    add_subdirectory(ext/pthread-win32)

	add_executable(fightable ${FIGHTABLE_SRC})
	add_compile_definitions(TARGET_WIN32)
    add_compile_definitions(__MINGW32__)

    set(FIGHTABLE_NATIVE_LIBS pthreadVC3 Ws2_32)
    set(FIGHTABLE_NATIVE_INCLUDES ext/pthread-win32 ext/strings_h)
elseif(UNIX)
    message(STATUS "Compiling for UNIX-like operating systems")

    set(FIGHTABLE_TARGET UNIX)

	add_executable(fightable ${FIGHTABLE_SRC})
    add_compile_definitions(TARGET_UNIX)
else()
    message(STATUS "Compiling for console homebrews")
    set(FIGHTABLE_TARGET Unknown homebrew)
	
    message(FATAL_ERROR "Support for platforms other than UNIX and NT is not implemented yet!")

    add_compile_definitions(TARGET_HOMEBREW_UNKNOWN)
endif()

add_library(ftcpsrv STATIC ${FIGHTABLE_TCP_SERVER_SRC})
target_link_libraries(ftcpsrv ${FIGHTABLE_NATIVE_LIBS})
target_include_directories(ftcpsrv PRIVATE
    ${FIGHTABLE_SHARED_INC}
    ${FIGHTABLE_NATIVE_INCLUDES}
)

add_executable(ftcpsrv-test src/tcpsrv_test/main.c)
target_link_libraries(ftcpsrv-test ftcpsrv ${FIGHTABLE_NATIVE_LIBS})
target_include_directories(ftcpsrv-test PRIVATE
    src/tcpsrv/inc
    ${FIGHTABLE_SHARED_INC}
    ${FIGHTABLE_NATIVE_INCLUDES}
)

add_executable(tcpserver-raylib-test src/tcpserver_raylib/main.c)
target_link_libraries(tcpserver-raylib-test ftcpsrv ${FIGHTABLE_NATIVE_LIBS})
target_include_directories(tcpserver-raylib-test PRIVATE
    src/tcpsrv/inc
    ${FIGHTABLE_SHARED_INC}
    ${FIGHTABLE_NATIVE_INCLUDES}
)

add_library(ftcpclient STATIC ${FIGHTABLE_TCP_CLIENT_SRC})
target_link_libraries(ftcpclient fshared ${FIGHTABLE_NATIVE_LIBS})
target_include_directories(ftcpclient PRIVATE
    src/tcpclient/inc
    ${FIGHTABLE_SHARED_INC}
    ${FIGHTABLE_NATIVE_INCLUDES}
)

add_executable(ftcpclient-test src/tcpclient_test/main.c)
target_link_libraries(ftcpclient-test ftcpclient ${FIGHTABLE_NATIVE_LIBS} m)
target_include_directories(ftcpclient-test PRIVATE
    src/tcpclient/inc
    ${FIGHTABLE_SHARED_INC}
    ${FIGHTABLE_NATIVE_INCLUDES}
)

add_executable(tcpclient-raylib-test src/tcpclient_raylib/main.c)
target_link_libraries(tcpclient-raylib-test raylib ftcpclient ${FIGHTABLE_NATIVE_LIBS} m)
target_include_directories(tcpclient-raylib-test PRIVATE
    src/tcpclient/inc
    ext/raylib/src
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
endif()

file(COPY ${CMAKE_SOURCE_DIR}/src/assets DESTINATION ${CMAKE_BINARY_DIR}/)

target_include_directories(ftcpsrv PRIVATE
    src/tcpsrv/inc
    ext/base64/include
    ${FIGHTABLE_NATIVE_INCLUDES}
    ${FIGHTABLE_SHARED_INC}
)

add_executable(physics-test
    src/physics_test/main.c
)

add_executable(collide-test
    src/collide_test/main.c
    src/collide_test/hitbox/clip_x.c
    src/collide_test/hitbox/clip_y.c
    src/collide_test/hitbox/move.c
)

target_include_directories(fightable PRIVATE 
    src/client/inc 
    ext/raylib/src 
    ext/cJSON 
    ext/portaudio_opensles/include
    ${FIGHTABLE_HTTP_INC}
    ${OPENMPT_INC}
    ${FIGHTABLE_NATIVE_INCLUDES}
    ${FIGHTABLE_SHARED_INC}
)

target_include_directories(physics-test PRIVATE ext/raylib/src ${FIGHTABLE_SHARED_INC})
target_include_directories(collide-test PRIVATE ext/raylib/src src/collide_test/inc ${FIGHTABLE_SHARED_INC})

target_link_libraries(fightable raylib cjson PortAudio ${OPENMPT_LIB} ${FIGHTABLE_NATIVE_LIBS} ${FIGHTABLE_HTTP_LINK} ftcpsrv fshared)
target_link_libraries(physics-test raylib)
target_link_libraries(collide-test raylib)


message(STATUS "")
message(STATUS "")

message(STATUS "           Fightable Compile Settings")
if (DISABLE_MP_SERVER)
    message(STATUS "  MP server IS disabled")
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

if (FIGHTABLE_CUDA)
    message(STATUS "  CUDA support: ${FIGHTABLE_CUDA}")
endif()

message(STATUS "")
message(STATUS "")