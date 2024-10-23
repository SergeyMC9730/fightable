#include <fightable/editor.hpp>
#include <fightable/editor.h>
#include <fightable/state.h>
#include <fightable/camera.h>
#include <fightable/tilemap.h>
#include <fightable/block_library.h>
#include <fightable/button.h>
#include <stdio.h>
#include <cmath>
#include <optional>

void _fEditorDraw(struct feditor *editor) {
    std::optional<fblock> selected_object = std::nullopt;
    bool mouse_out_of_bounds = false;

    editor->level.in_workbench_mode = !editor->should_playback;
    if (editor->level.in_workbench_mode) {
        editor->entities.clear();
    }

    if (editor->should_process_interactions) {
        float delta = GetFrameTime();
        float speed = 60.f;

        if (IsKeyDown(KEY_LEFT_SHIFT)) {
            speed *= 1.6f;
        }

        float delta_speed = delta * speed;

        if (IsKeyDown(KEY_A)) {
            editor->camera.target.x -= delta_speed;
        }
        if (IsKeyDown(KEY_D)) {
            editor->camera.target.x += delta_speed;
        }
        if (IsKeyDown(KEY_W)) {
            editor->camera.target.y -= delta_speed;
        }
        if (IsKeyDown(KEY_S)) {
            editor->camera.target.y += delta_speed;
        }

        editor->level.camera = editor->camera;

        editor->render_objects.clear();
        for (auto &[x, ys] : editor->objects) {
            for (auto &[y, _obj] : ys) {
                _obj.base.block_x = x;
                _obj.base.block_y = y;

                editor->render_objects.push_back(_obj);
            }
        }

        editor->level.objects = editor->render_objects.data();
        editor->level.data_size = editor->render_objects.size();
    }

    editor->level.entities = editor->entities.data();
    editor->level.entity_data_size = editor->entities.size();

    _fLevelDraw(&editor->level, {0, 0});

    Color bouncing_color = RED;
    float v = 128.f * ((std::sin(__state.time * 3.f) / 2.f) + 0.5f) + 15.f;

    bouncing_color.a = (unsigned char)(std::max(0.f, std::min(v, 255.f)));

    if (editor->should_process_interactions) {
        Camera2D actual_cam = editor->level.camera;
        actual_cam.target.x = (int)actual_cam.target.x;
        actual_cam.target.y = (int)actual_cam.target.y;

        Vector2 mouse_pos = GetMousePosition();
        mouse_pos.x /= 5; mouse_pos.y /= 5;

        Vector2 m_world_pos = mouse_pos;

        m_world_pos.x += actual_cam.target.x;
        m_world_pos.y += actual_cam.target.y;

        float tx = editor->level.tilemap->tile_size.x;
        float ty = editor->level.tilemap->tile_size.y;

        if (mouse_pos.x > (800.f / 5.f)) {
            mouse_out_of_bounds = true;
        }

        Vector2 mapped_block_pos = {
            std::floor(m_world_pos.x / tx) * tx,
            std::floor(m_world_pos.y / ty) * ty,
        };

        IVector2 selected_block_pos = {
            (int)(mapped_block_pos.x / tx),
            (int)(mapped_block_pos.y / ty)
        };

        if (editor->objects.count(selected_block_pos.x)) {
            if (editor->objects[selected_block_pos.x].count(selected_block_pos.y)) {
                selected_object = editor->objects[selected_block_pos.x][selected_block_pos.y];
            }
        }

        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT) && !mouse_out_of_bounds) {
            auto block = _fBlockFromId(editor->current_block_id);

            if (block.singular && _fEditorContainsId(editor, editor->current_block_id)) {
                IVector2 pos = _fEditorGetPosOfFirstId(editor, editor->current_block_id);
                editor->objects[pos.x][pos.y] = _fBlockFromId(0);
            }

            editor->objects[selected_block_pos.x][selected_block_pos.y] = block;

            // printf("placed block at %d:%d\n", selected_block_pos.x, selected_block_pos.y);
        }

        {
            BeginMode2D(actual_cam);

            if (!mouse_out_of_bounds) {
                DrawRectangleV(mapped_block_pos, {tx, ty}, bouncing_color);
            }
            // DrawPixel(mouse_pos.x, mouse_pos.y, RED);

#define MAX_BLOCK_AREA ((0xFFFF + 1) * 8)

            DrawRectangleLinesEx({-MAX_BLOCK_AREA / 2, -MAX_BLOCK_AREA / 2, MAX_BLOCK_AREA / 1, MAX_BLOCK_AREA / 1}, 3.f, bouncing_color);

            EndMode2D();

            char buf[16] = {};
            snprintf(buf, 16, "%d:%d", (int)actual_cam.target.x, (int)actual_cam.target.y);

            _fTextDraw(&__state.text_manager, buf, {4, 4}, GREEN, 1);
        }
    
        float v = GetMouseWheelMove();
        if (v > 0.f) {
            editor->current_block_id = (editor->current_block_id + 1) % editor->block_listing.total;
        } else if (v < 0.f) {
            if (editor->current_block_id == 0) {
                editor->current_block_id = editor->block_listing.total - 1;
            } else {
                editor->current_block_id = (editor->current_block_id - 1) % editor->block_listing.total;
            }
        }
    }

    if (editor->should_display_sidebar) {
        int blackbox_startx = GetRenderWidth() / 5 - 51;
        int blackbox_starty = 0;
        int space = GetRenderWidth() / 5 - blackbox_startx;

        DrawRectangle(GetRenderWidth() / 5 - 51, 0, 51, GetRenderHeight() / 5, (Color){0, 0, 0, 200});

        IVector2 sel_block_len = _fTextMeasure(&__state.text_manager, "sel block");
        IVector2 none_len = _fTextMeasure(&__state.text_manager, "none");

        int center = (space - sel_block_len.x) / 2;

        _fTextDraw(&__state.text_manager, "sel block", {blackbox_startx + center, blackbox_starty + 4}, GREEN, 1);

        if (!selected_object.has_value()) {
            center = (space - none_len.x) / 2;
            
            _fTextDraw(&__state.text_manager, "none", {blackbox_startx + center, blackbox_starty + 14}, RED, 1);
        } else {
            fblock obj = selected_object.value();
            
            center = (space - editor->level.tilemap->tile_size.x) / 2;

            _fTilemapDraw(*editor->level.tilemap, {center + blackbox_startx, blackbox_starty + 12}, {obj.base.tile_x, obj.base.tile_y}, 0, 0, WHITE);
        }

        Color grad_black = (Color){};
        Color grad_gray = GRAY;

        DrawRectangleGradientH(blackbox_startx, blackbox_starty + 23, space / 2, 1, grad_black, grad_gray);
        DrawRectangleGradientH(blackbox_startx + (space / 2), blackbox_starty + 23, space / 2, 1, grad_gray, grad_black);

        center = (space - sel_block_len.x) / 2;
        _fTextDraw(&__state.text_manager, "set block", {blackbox_startx + center, blackbox_starty + 27}, YELLOW, 1);

        fblock block = _fBlockFromId(editor->current_block_id);

        _fTilemapDraw(*editor->level.tilemap, {blackbox_startx + 4, blackbox_starty + 36}, {block.base.tile_x, block.base.tile_y}, 0, 0, WHITE);
        
        char buf[8] = {};
        snprintf(buf, 8, "%d", (int)editor->current_block_id);

        _fTextDraw(&__state.text_manager, buf, {blackbox_startx + 15, blackbox_starty + 38}, GRAY, 1);
        _fTextDraw(&__state.text_manager, block.name, {blackbox_startx + 28, blackbox_starty + 38}, LIGHTGRAY, 1);

        DrawRectangleGradientH(blackbox_startx, blackbox_starty + 46, space / 2, 1, grad_black, grad_gray);
        DrawRectangleGradientH(blackbox_startx + (space / 2), blackbox_starty + 46, space / 2, 1, grad_gray, grad_black);

        sel_block_len = _fTextMeasure(&__state.text_manager, "render");
        center = (space - sel_block_len.x) / 2;
        _fTextDraw(&__state.text_manager, "render", {blackbox_startx + center, blackbox_starty + 50}, RED, 1);

        snprintf(buf, 8, "%d", editor->level.objects_rendered);
        sel_block_len = _fTextMeasure(&__state.text_manager, buf);
        center = (space - sel_block_len.x) / 2;

        _fTextDraw(&__state.text_manager, buf, {blackbox_startx + center, blackbox_starty + 57}, DARKGRAY, 1);

        DrawRectangleGradientH(blackbox_startx, blackbox_starty + 64, space / 2, 1, grad_black, grad_gray);
        DrawRectangleGradientH(blackbox_startx + (space / 2), blackbox_starty + 64, space / 2, 1, grad_gray, grad_black);

        if (_fEditorContainsId(editor, BLOCK_START)) {
            struct fbutton btn = {};
            btn.text = "Play";
            
            btn.position.x = blackbox_startx + 14;
            btn.position.y = blackbox_starty + 66;
            
            if (_fButtonDraw(&btn) || IsKeyPressed(KEY_F1)) {
                printf("click\n");

                IVector2 pos = _fEditorGetPosOfFirstId(editor, BLOCK_START);

                editor->should_display_sidebar = false;
                editor->should_process_interactions = false;
                editor->should_playback = true;

                struct fentity player = {.global_entity_id = 1};
                player.hitbox.width = editor->level.tilemap->tile_size.x;
                player.hitbox.height = editor->level.tilemap->tile_size.y;
                player.hitbox.x = pos.x * player.hitbox.width;
                player.hitbox.y = pos.y * player.hitbox.height;
                player.debug_draw = 1;

                editor->entities.push_back(player);

                SetWindowSize(800, 600);
                UnloadRenderTexture(__state.framebuffer);
                __state.framebuffer = LoadRenderTexture(800 / 5, 600 / 5);

                editor->f1_lock = true;
            }
        }
    }

    if (editor->should_playback) {
        _fTextDraw(&__state.text_manager, "f1 to exit", {2, 2}, BLUE, 1);

        if (IsKeyPressed(KEY_F1) && !editor->f1_lock) {
            editor->should_display_sidebar = true;
            editor->should_process_interactions = true;
            editor->should_playback = false;
            editor->level.entities = 0;

            SetWindowSize(800 + 255, 600);
            UnloadRenderTexture(__state.framebuffer);
            __state.framebuffer = LoadRenderTexture((800 + 255) / 5, 600 / 5);
        }

        editor->f1_lock = false;
    }
}
