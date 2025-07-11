
//          Sergei Baigerov 2024 - 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#define WITH_PLACEHOLDERS

#include <fightable/level.h>
#include <fightable/tilemap.h>
#include <fightable/state.h>
#include <fightable/block.h>
#include <fightable/entity.h>
#include <fightable/color.h>
#include <fightable/sanitizer.h>
#include <fightable/renderer.h>

RSB_ARRAY_IMPL_GEN(struct fentity*, _fentity);
RSB_ARRAY_IMPL_GEN(struct flevel_light_source, _lls);
RSB_ARRAY_IMPL_GEN(struct flevel_registry_entry, _lre);

extern double _rendererOutQuint(double x);

void _fLevelDraw(struct flevel *level, IVector2 initial_pos) {
    if (!level || !level->tilemap) return;

    if (IsKeyPressed(KEY_P)) {
        level->pause_world = !level->pause_world;
    }

    if (level->light_sources) {
        level->light_sources->added_elements = 0;
        level->light_sources->current_index = 0;
    }
    else {
        level->light_sources = RSBCreateArray_lls();
    }

    const float delta = GetFrameTime();

    float gameover_rotation = level->cam_rot;
    float gameover_zoom = 1.f;
    float gameover_speed = 1.f / 3.f;
    float gameover_ratio = 0.f;
    Color gameover_bg = BLANK;

    if (level->in_gameover_mode) {
        gameover_ratio = _rendererOutQuint(level->gameover_time * gameover_speed);
        gameover_rotation = gameover_ratio * 30.f;
        gameover_zoom = 1 + (gameover_ratio * 1.5f);

        Color end_bg = RED;
        end_bg.a = 196;

        gameover_bg = _fMixColors(BLANK, end_bg, level->gameover_time * gameover_speed);

        level->gameover_time += delta;
        if (level->gameover_time > 1.f) {
            level->gameover_time = 1.f;
        }
    }

    struct fentity *player = _fLevelFindPlayer(level);

    Camera2D actual_cam = level->camera;
    actual_cam.target.x = (int)actual_cam.target.x + __state.gui_render_offset.x;
    actual_cam.target.y = (int)actual_cam.target.y + __state.gui_render_offset.y;
    actual_cam.rotation = gameover_rotation;
    actual_cam.zoom = gameover_zoom;

    level->render_cam = actual_cam;

    int tx = level->tilemap->tile_size.x;
    int ty = level->tilemap->tile_size.y;

    if (player) {
        actual_cam.target.x = (int)(player->hitbox.x - (float)__state.framebuffer.texture.width / 2) + (int)__state.gui_render_offset.x;
        actual_cam.target.y = (int)(player->hitbox.y - (float)__state.framebuffer.texture.height / 2) + (int)__state.gui_render_offset.y;
    }

    IVector2 cam_blocks = {
        (int)actual_cam.target.x / tx,
        (int)actual_cam.target.y / ty
    };
    actual_cam.target.x -= cam_blocks.x * tx;
    actual_cam.target.y -= cam_blocks.y * ty;

    // BeginMode2DStacked(actual_cam);

    {
        RLRectangle source = {0};
        source.width = __state.framebuffer.texture.width;
        source.height = __state.framebuffer.texture.height;
        source.x = (int)((actual_cam.target.x + cam_blocks.x * tx) / 1.5f) % level->background_tile.width;
        source.y = (int)((actual_cam.target.y + cam_blocks.y * ty) / 1.5f) % level->background_tile.height;

        RLRectangle dest = source;
        dest.x = actual_cam.target.x; dest.x = 0;
        dest.y = actual_cam.target.y; dest.y = 0;

        DrawTexturePro(level->background_tile, source, dest, (Vector2){0}, 0.f, GRAY);
    }

    if (__state.display_test_midground && IsTextureValid(__state.test_midground)) {
        RLRectangle source = {0};
        source.width = __state.framebuffer.texture.width;
        source.height = __state.framebuffer.texture.height;
        source.x = (int)(actual_cam.target.x / 1.5f) % __state.test_midground.width;
        source.y = (int)(actual_cam.target.y / 1.5f) % __state.test_midground.height;

        RLRectangle dest = source;
        dest.x = actual_cam.target.x; dest.x = 0;
        dest.y = actual_cam.target.y; dest.y = 0;

        DrawTexturePro(__state.test_midground, source, dest, (Vector2){0}, 0.f, (Color){255, 255, 255, 128});
    }

    level->objects_rendered = 0;

    // if (player) {
    //     actual_cam.target.x += (__state.framebuffer.texture.width / 2);
    //     actual_cam.target.y += (__state.framebuffer.texture.height / 2);
    // }

    RLRectangle area = (RLRectangle){
        (int)(-(actual_cam.target.x + (cam_blocks.x * tx))),
        (int)(-(actual_cam.target.y + (cam_blocks.y * ty))),
        level->camera_size.x,
        level->camera_size.y
    };

    level->cam_offset.x = area.x;
    level->cam_offset.y = area.y;

    DrawRectangleLinesEx(area, 1.f, YELLOW);

    // TraceLog(LOG_INFO, "%d %d | %f %f | %f %f %f %f", cam_blocks.x, cam_blocks.y, actual_cam.target.x, actual_cam.target.y, area.x, area.y, area.width, area.height);

    if (level->render_crop_area.width * level->render_crop_area.height > 0.f) {
        BeginScissorMode(level->render_crop_area.x, level->render_crop_area.x, level->render_crop_area.width, level->render_crop_area.height);
    }

    for (unsigned int i = 0; i < level->data_size; i++) {
        struct fblock *obj = level->objects + i;

        int _x = initial_pos.x + (obj->base.block_x * tx) + area.x;
        int _y = initial_pos.y + (obj->base.block_y * ty) + area.y;

        if (_x > __state.framebuffer.texture.width || _y > __state.framebuffer.texture.height || (_x + tx) < 0 || (_y + ty) < 0) continue;
        if (_fBlockIdFromRenderable(obj->base) == 0) continue;

        if (level->in_workbench_mode) {
            _fTilemapDraw(level->tilemap, (IVector2){_x + 1, _y + 1}, (IVector2){obj->base.tile_x, obj->base.tile_y}, obj->base.flipped_x, obj->base.flipped_y, BLACK);
        }
        _fTilemapDraw(level->tilemap, (IVector2){_x, _y}, (IVector2){obj->base.tile_x, obj->base.tile_y}, obj->base.flipped_x, obj->base.flipped_y, WHITE);
        if (player && player->damage_colddown > 0 && obj->dangerous) {
            Color col = RED;
            col.a = (unsigned char)(255.f * (1.f / player->max_damage_colddown) * player->damage_colddown * 0.85f);

            DrawRectangleLines(_x, _y, tx, ty, col);
        }
        if (obj->light_level != 0) {
            Color col = WHITE;
            col.a = obj->light_level;

            int cx = _x;
            int cy = _y;

            struct flevel_light_source source = {
                .pos = (IVector2){cx, cy},
                .tint = col
            };

            RSBAddElement_lls(level->light_sources, source);
        }

        level->objects_rendered++;
    }

    if (level->entities && level->hitboxes) {
        if (player) {
            player->custom_delta = 0.f;
            player->obstacles = level->hitboxes;
            player->obstacles_length = level->data_size;
            player->update(player);
        }

        for (int i = 0; i < level->entities->added_elements; i++) {
            struct fentity* entity = level->entities->objects[i];
            if (!entity) continue;

            if (!entity->draw) {
                _fEntityDraw(entity);
            }
            else {
                entity->draw(entity);
            }
        }
    }

    if (level->render_crop_area.width * level->render_crop_area.height > 0.f) {
        EndScissorMode();
    }

    BeginBlendMode(BLEND_ADDITIVE);

    for (unsigned int i = 0; i < level->light_sources->added_elements; i++) {
        struct flevel_light_source* source = level->light_sources->objects + i;
        if (player) {
            if (!CheckCollisionPointRec((Vector2) { source->pos.x, source->pos.y }, player->hitbox)) {
                _fLevelLightSourceDraw(level, source);
            }
        }
        else {
            _fLevelLightSourceDraw(level, source);
        }
    }

    EndBlendMode();

    // EndMode2DStacked();

    if (IsKeyPressed(KEY_M) && level->entities) {
        TraceLog(LOG_INFO, "Damaging all entities by 0%");

        for (int i = 0; i < level->entities->added_elements; i++) {
            struct fentity* entity = RSBGetAtIndex_fentity(level->entities, i);
            if (!entity) continue;

            if (!entity->damage) {
                _fEntityDamage(entity, 0.f);
            }
            else {
                entity->damage(entity, 0.f);
            }
        }
    }

    DrawRectangle(0, 0, GetRenderWidth(), GetRenderHeight(), gameover_bg);

#ifdef TARGET_SUPPORTS_3D
    // float camfov = 45.f;

    // Camera camera = { { 0.0f, 0.0f, 12.f / sin(camfov * (DEG2RAD))}, {0.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, camfov, 0 };
    // camera.position.x = actual_cam.target.x / (float)level->tilemap->tile_size.x;
    // camera.target.x = actual_cam.target.x / (float)level->tilemap->tile_size.x;
    // camera.position.y = -actual_cam.target.y / (float)level->tilemap->tile_size.y;
    // camera.target.y = -actual_cam.target.y / (float)level->tilemap->tile_size.y;

    // BeginMode3D(camera);

    // DrawGridEx(10, 1.0f, 0.f);
    // DrawGridEx(10, 1.0f, -4.f);
    // DrawGridEx(10, 1.0f, -8.f);
    // DrawGridEx(10, 1.0f, -12.f);

    // EndMode3D();
#endif

    // DrawRectangleRec(area, (Color){255, 255, 255, 64});

    // DrawTexture(level->background_tile, 0, 0, WHITE);

    // TraceLog(LOG_INFO, "Rendered: %d", level->objects_rendered);
}
