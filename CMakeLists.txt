cmake_minimum_required(VERSION 3.28)
project(fightable)

set(BUILD_SHARED_LIBS OFF)

add_subdirectory(ext/cJSON)
add_subdirectory(ext/raylib)

add_executable(fightable 
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
)

add_executable(physics-test
    src/physics_test/main.c
)

add_executable(helloworld
    src/helloworld/main.c
)

add_library(raylib-statistics SHARED
    src/raylib_statistics/state.c
    src/raylib_statistics/texture.c
    src/raylib_statistics/test.c
)

target_include_directories(fightable PRIVATE src/client/inc ext/raylib/src ext/cJSON)
target_include_directories(physics-test PRIVATE ext/raylib/src)
target_include_directories(raylib-statistics PRIVATE ext/raylib/src src/raylib_statistics/inc)

set_property(TARGET raylib-statistics PROPERTY POSITION_INDEPENDENT_CODE ON)
target_compile_definitions(raylib-statistics PRIVATE _GNU_SOURCE=1)

target_link_libraries(fightable raylib cjson openmpt portaudio)
target_link_libraries(physics-test raylib)
target_link_libraries(raylib-statistics raylib dl)