cmake_minimum_required(VERSION 3.28)
project(fightable)

find_library(OPENMPT_LIB openmpt)
if(NOT OPENMPT_LIB)
    message(FATAL_ERROR "libopenmpt could not be found")
endif()
find_library(MICROHTTPD_LIB microhttpd)
if(NOT MICROHTTPD_LIB)
    message(FATAL_ERROR "libmicrohttpd could not be found")
endif()

set(CMAKE_CXX_STANDARD 20)

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

CHECK_INCLUDE_FILE("libopenmpt/libopenmpt.h" HAVE_LIBOPENMPT)
CHECK_INCLUDE_FILE("microhttpd.h" HAVE_LIBMICROHTTPD)

if (NOT HAVE_LIBOPENMPT)
	message(FATAL_ERROR "libopenmpt binaries were found but the header files itself are missing")
endif()
if (NOT HAVE_LIBMICROHTTPD)
	message(FATAL_ERROR "libmicrohttpd binaries were found but the header files itself are missing")
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
)

file(GLOB HTTPSERVER_SRC
    "ext/libhttpserver/src/*.hpp"
    "ext/libhttpserver/src/*.h"
    "ext/libhttpserver/src/*.cpp"

    "ext/libhttpserver/src/details/*.cpp"

    "ext/libhttpserver/src/httpserver/*.hpp"
    "ext/libhttpserver/src/httpserver/details/*.hpp"
)

set(HTTPSERVER_INC
    ext/libhttpserver/src
    ext/libhttpserver/src/details
    ext/libhttpserver/src/httpserver
    ext/libhttpserver/src/httpserver/details
)

add_library(httpserver STATIC ${HTTPSERVER_SRC})
target_include_directories(httpserver PRIVATE ${HTTPSERVER_INC})
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
)
target_include_directories(fightable-http PRIVATE
    src/http/inc
    ${HTTPSERVER_INC}
)
target_link_libraries(fightable-http PUBLIC httpserver)

if (ANDROID)
	set(FIGHTABLE_SRC
		${FIGHTABLE_SRC}
		$ENV{ANDROID_NDK}/sources/android/native_app_glue/android_native_app_glue.c
	)

	include_directories($ENV{ANDROID_NDK}/sources/android/native_app_glue)

	add_library(fightable SHARED ${FIGHTABLE_SRC})
	set(FIGHTABLE_NATIVE_LIBS android log)
	target_compile_definitions(fightable PUBLIC TARGET_ANDROID)
    target_compile_definitions(fightable PUBLIC TARGET_UNIX)
elseif(WIN32)
	add_executable(fightable ${FIGHTABLE_SRC})
	target_compile_definitions(fightable PUBLIC TARGET_WIN32)
else()
	add_executable(fightable ${FIGHTABLE_SRC})
        target_compile_definitions(fightable PUBLIC TARGET_UNIX)
endif()

add_executable(physics-test
    src/physics_test/main.c
)

target_include_directories(fightable PRIVATE src/client/inc ext/raylib/src ext/cJSON ext/portaudio_opensles/include src/http/inc)
target_include_directories(physics-test PRIVATE ext/raylib/src)

target_link_libraries(fightable raylib cjson PortAudio ${OPENMPT_LIB} ${FIGHTABLE_NATIVE_LIBS} fightable-http)
target_link_libraries(physics-test raylib)
