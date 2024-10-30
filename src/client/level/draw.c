#define WITH_PLACEHOLDERS

#include <fightable/level.h>
#include <fightable/tilemap.h>
#include <fightable/state.h>
#include <fightable/block.h>
#include <fightable/entity.h>
#include <stdio.h>
#include <math.h>

void _fLevelDraw(struct flevel *level, IVector2 initial_pos) {
    if (!level || !level->tilemap) return;

    Rectangle *rects = 0;
    struct fentity *player = 0;

    if (level->entities && level->entity_data_size != 0) {
        rects = _fLevelGetHitboxes(level);
        player = _fLevelFindPlayer(level);
    }

    Camera2D actual_cam = level->camera;
    actual_cam.target.x = (int)actual_cam.target.x;
    actual_cam.target.y = (int)actual_cam.target.y;

    int tx = level->tilemap->tile_size.x;
    int ty = level->tilemap->tile_size.y;

    if (player) {
        actual_cam.target.x = (int)player->hitbox.x - (int)(__state.framebuffer.texture.width / 2);
        actual_cam.target.y = (int)player->hitbox.y - (int)(__state.framebuffer.texture.height / 2);

        level->camera_size.x = __state.framebuffer.texture.width * tx;
        level->camera_size.y = __state.framebuffer.texture.height * ty;

        // printf("%d:%d\n", (int)actual_cam.target.x, (int)actual_cam.target.y);
    }

    BeginMode2D(actual_cam);
    
    Rectangle source = {};
    source.width = __state.framebuffer.texture.width;
    source.height = __state.framebuffer.texture.height;
    source.x = (int)(actual_cam.target.x / 1.5f) % level->background_tile.width;
    source.y = (int)(actual_cam.target.y / 1.5f) % level->background_tile.height;

    Rectangle dest = source;
    dest.x = actual_cam.target.x;
    dest.y = actual_cam.target.y;
    
    DrawTexturePro(level->background_tile, source, dest, (Vector2){}, 0.f, DARKGRAY);

    level->objects_rendered = 0;

    // if (player) {
    //     actual_cam.target.x += (__state.framebuffer.texture.width / 2);
    //     actual_cam.target.y += (__state.framebuffer.texture.height / 2);
    // }

    Rectangle area = (Rectangle){
        actual_cam.target.x - tx,
        actual_cam.target.y - tx,
        level->camera_size.x + tx,
        level->camera_size.y + tx
    };

    if (level->in_workbench_mode) {
        for (unsigned int i = 0; i < level->data_size; i++) {
            struct fblock obj = level->objects[i];

            if (_fBlockIdFromRenderable(obj.base) == 0) continue;
            if (!CheckCollisionPointRec((Vector2){obj.base.block_x * tx, obj.base.block_y * ty}, area)) continue;

            int _x = initial_pos.x + (obj.base.block_x * tx);
            int _y = initial_pos.y + (obj.base.block_y * ty);

            _fTilemapDraw(level->tilemap, (IVector2){_x + 1, _y + 1}, (IVector2){obj.base.tile_x, obj.base.tile_y}, obj.base.flipped_x, obj.base.flipped_y, BLACK);
        }
    }

    for (unsigned int i = 0; i < level->data_size; i++) {
        struct fblock obj = level->objects[i];

        if (_fBlockIdFromRenderable(obj.base) == 0) continue;
        if (!CheckCollisionPointRec((Vector2){obj.base.block_x * tx, obj.base.block_y * ty}, area)) continue;

        int _x = initial_pos.x + (obj.base.block_x * tx);
        int _y = initial_pos.y + (obj.base.block_y * ty);

        _fTilemapDraw(level->tilemap, (IVector2){_x, _y}, (IVector2){obj.base.tile_x, obj.base.tile_y}, obj.base.flipped_x, obj.base.flipped_y, WHITE);

        level->objects_rendered++;
    }

    if (level->entities && rects) {
        float delta = GetFrameTime();

        // for (int i = 0; i < level->data_size; i++) {
        //     Color col = RED;
        //     col.a = 128;

        //     Rectangle r = rects[i];

        //     DrawRectangleRec(r, col);
        // }

        for (int i = 0; i < level->entity_data_size; i++) {
            struct fentity *entity = level->entities + i;

            entity->obstacles = rects;
            entity->obstacles_length = level->data_size;
            entity->delta = delta;

            if (entity->global_entity_id == 1) {
                entity->walking = 0;
                entity->jumping = 0;

                if (IsKeyDown(KEY_A)) {
                    entity->walking = 1;
                    entity->negative_move = 1;
                    entity->render_direction = ENTITY_DIR_LEFT;
                } else if (IsKeyDown(KEY_D)) {
                    entity->walking = 1;
                    entity->negative_move = 0;
                    entity->render_direction = ENTITY_DIR_RIGHT;
                }

                if (IsKeyPressed(KEY_SPACE)) {
                    entity->jumping = 1;
                    entity->accel_y = -138;
                }
            }
        
            if (!entity->update) {
                _fEntityUpdate(entity);
            }
            if (!entity->draw && entity != player) {
                _fEntityDraw(entity);
            }
        }
    }

    EndMode2D();

    if (player) {
        IVector2 rpos = {
            .x = (__state.framebuffer.texture.width - player->hitbox.width) / 2 + 4,
            .y = (__state.framebuffer.texture.height - player->hitbox.height) / 2 + 3
        };

        _fTilemapDraw(level->tilemap, rpos, (IVector2){12, 0}, (player->render_direction == ENTITY_DIR_LEFT) ? 1 : 0, 0, WHITE);

        rpos.y -= (level->tilemap->tile_size.y - 2);
        rpos.x--;

        if (player->render_direction == ENTITY_DIR_LEFT) {
            rpos.x += 2;
        }

        _fTilemapDraw(level->tilemap, rpos, (IVector2){11, 1}, (player->render_direction == ENTITY_DIR_LEFT) ? 1 : 0, 0, WHITE);

        //DrawRectangle((__state.framebuffer.texture.width - player->hitbox.width) / 2 + 5, (__state.framebuffer.texture.height - player->hitbox.height) / 2 + 3, player->hitbox.width, player->hitbox.height, RED);
    }

    if (rects) {
        MemFree(rects);
    }

    // DrawTexture(level->background_tile, 0, 0, WHITE);
}