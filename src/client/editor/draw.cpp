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
#include <fightable/renderer.h>
#include <fightable/intvec.h>
#include <fightable/player.h>
bool block_select = false;
void _fEditorDraw(struct feditor *editor) {
    std::optional<fblock> selected_object = std::nullopt;
    bool mouse_out_of_bounds = false;

    if (IsKeyDown(KEY_LEFT_SHIFT)) {
        editor->swipe_enabled = 1;
    } else {
        editor->swipe_enabled = 0;
    }

    editor->level.in_workbench_mode = !editor->should_playback;
    if (editor->level.in_workbench_mode) {
        editor->entities.clear();
    }

    float speed = 1.f;
    RLRectangle mobile_swipe_area = {};

    mobile_swipe_area.width = 8;
    mobile_swipe_area.height = 32;
    mobile_swipe_area.x = 4;
    mobile_swipe_area.y = __state.framebuffer.texture.height - mobile_swipe_area.height - 4;

    // printf("a\n");

    if (editor->should_process_interactions && !editor->should_display_selector) {
        float delta = GetFrameTime();

        if (IsKeyDown(KEY_RIGHT_SHIFT)) {
            speed *= 1.6f;
        }
        float delta_speed = delta * speed;

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

    if (!editor->should_playback) {
        editor->level.render_crop_area = { 0, 0, 160, 120 };
    }
    else {
        editor->level.render_crop_area = { 0, 0, 0, 0 };
    }

    _fLevelDraw(&editor->level, {0, 0});

    Color bouncing_color = RED;
    float v = 128.f * ((std::sin(__state.time * 3.f) / 2.f) + 0.5f) + 15.f;

    bouncing_color.a = (unsigned char)(std::max(0.f, std::min(v, 255.f)));
    
    
    Vector2 mouse_pos = _fGetMousePosPix();

    // printf("b\n");

    if (editor->should_process_interactions && !editor->should_display_selector) {
        Camera2D actual_cam = editor->level.camera;
        actual_cam.target.x = (int)actual_cam.target.x;
        actual_cam.target.y = (int)actual_cam.target.y;

        Vector2 m_world_pos = mouse_pos;

        m_world_pos.x += actual_cam.target.x;
        m_world_pos.y += actual_cam.target.y;

        // printf("pos: %f:%f\n", m_world_pos.x, m_world_pos.y);

        float tx = editor->level.tilemap->tile_size.x;
        float ty = editor->level.tilemap->tile_size.y;

        if (mouse_pos.x > 160 || mouse_pos.x < 0) {
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

        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            editor->holded_previosly = 0;
        }

#ifdef TARGET_ANDROID
        mobile_swipe_area.height /= 2;

        DrawRectangleRounded(mobile_swipe_area, 2.f, 8.f, {253, 249, 0, 128});
        DrawRectangleRoundedLines(mobile_swipe_area, 1.f, 8.f, YELLOW);

        if (CheckCollisionPointRec(mouse_pos, mobile_swipe_area)) {
            mouse_out_of_bounds = true;

            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                _fEditorSwipeCurrentObjects(editor, 1.f);
            }
        }

        mobile_swipe_area.y += mobile_swipe_area.height;

        DrawRectangleRounded(mobile_swipe_area, 2.f, 8.f, {255, 64, 64, 128});
        DrawRectangleRoundedLines(mobile_swipe_area, 1.f, 8.f, RED);

        if (CheckCollisionPointRec(mouse_pos, mobile_swipe_area)) {
            mouse_out_of_bounds = true;

            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                _fEditorSwipeCurrentObjects(editor, -1.f);
            }
        }
#endif
        if(selected_object.has_value() && IsKeyPressed(KEY_DELETE)) {
            _fEditorPlaceBlock(editor, BLOCK_AIR, selected_block_pos);
        }
        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT) && !mouse_out_of_bounds) {
            if (editor->swipe_enabled) {
                _fEditorPlaceBlock(editor, editor->current_block_id, selected_block_pos);
            } else {
                Vector2 mdelta = GetMouseDelta();

                if (mdelta.x * mdelta.y != 0.f) {
                    editor->holded_previosly = 1;
                }

                mdelta.x /= __state.window_scale / speed;
                mdelta.y /= __state.window_scale / speed;
            
                editor->camera.target.x -= mdelta.x;
                editor->camera.target.y -= mdelta.y;
            }
        }
        if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT) && !editor->swipe_enabled && !editor->holded_previosly && !mouse_out_of_bounds) {
            _fEditorPlaceBlock(editor, editor->current_block_id, selected_block_pos);
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
        _fEditorSwipeCurrentObjects(editor, v);
    }

    // printf("c\n");

    if (editor->should_display_sidebar) {
        int blackbox_startx = (__state.framebuffer.texture.width) - 51;
        int blackbox_starty = 0;
        int space = __state.framebuffer.texture.width - blackbox_startx;

        int current_position_y = blackbox_starty;

        DrawRectangle(__state.framebuffer.texture.width - 51, 0, 51, __state.framebuffer.texture.height, Color{0, 0, 0, 100});

        IVector2 sel_block_len = _fTextMeasure(&__state.text_manager, "sel block");
        IVector2 none_len = _fTextMeasure(&__state.text_manager, "none");

        int center = (space - sel_block_len.x) / 2;
        struct fbutton btnBlock = {};
        btnBlock.text = "Blocks";
        
        btnBlock.position.x = blackbox_startx + 10;
        btnBlock.position.y = blackbox_starty + 66;
        btnBlock.tint = WHITE;
        if(_fButtonDraw(&btnBlock) || IsKeyPressed(KEY_F2)) {
            editor->should_display_selector = ~editor->should_display_selector;
        }
        _fTextDraw(&__state.text_manager, "sel block", {blackbox_startx + center, current_position_y + 4}, GREEN, 1);
        current_position_y += 4 + sel_block_len.y;

        if (!selected_object.has_value()) {
            center = (space - none_len.x) / 2;
            
            _fTextDraw(&__state.text_manager, "none", {blackbox_startx + center, current_position_y + 4}, RED, 1);
            current_position_y += 5 + none_len.y;
        } else {
            fblock obj = selected_object.value();
            
            center = (space - editor->level.tilemap->tile_size.x) / 2;

            _fTilemapDraw(editor->level.tilemap, {center + blackbox_startx, current_position_y + 2}, {obj.base.tile_x, obj.base.tile_y}, 0, 0, WHITE);
            current_position_y += 2 + __state.tilemap->tile_size.y;
        }

        Color grad_black = BLANK;
        Color grad_gray = GRAY;

        DrawRectangleGradientH(blackbox_startx, current_position_y + 2, space / 2, 1, grad_black, grad_gray);
        DrawRectangleGradientH(blackbox_startx + (space / 2), current_position_y + 2, space / 2, 1, grad_gray, grad_black);
        current_position_y += 4;

        center = (space - sel_block_len.x) / 2;
        _fTextDraw(&__state.text_manager, "set block", {blackbox_startx + center, current_position_y + 3}, YELLOW, 1);
        current_position_y += 6 + sel_block_len.y;

        fblock block = _fBlockFromId(editor->current_block_id);

        _fTilemapDraw(editor->level.tilemap, {blackbox_startx + 4, current_position_y + 1}, {block.base.tile_x, block.base.tile_y}, 0, 0, WHITE);
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
            btn.position.y = blackbox_starty + 75;
            btn.tint = WHITE;
            
            if (_fButtonDraw(&btn) || IsKeyPressed(KEY_F1)) {
                printf("click\n");
                block_select = false;
                IVector2 pos = _fEditorGetPosOfFirstId(editor, BLOCK_START);

                editor->should_display_sidebar = false;
                editor->should_process_interactions = false;
                editor->should_playback = true;

                felplayer* player = (felplayer*)MemAlloc(sizeof(felplayer));

                _flPlayerInit(player);
                _fEntitySetPosition(&player->base, { pos.x * player->base.hitbox.width, pos.y * player->base.hitbox.height });

                editor->entities.push_back((fentity *)player);

                Vector2 dpi = GetWindowScaleDPI();
                Vector2 wanted_resolution = {
                    .x = 160,
                    .y = 120
                };

                // SetWindowSize(__state.base_game_size.x - __state.editor_size.x, __state.base_game_size.y - __state.editor_size.y);
                UnloadRenderTexture(__state.framebuffer);
                UnloadRenderTexture(__state.overlay_framebuffer);

                __state.framebuffer = LoadRenderTexture(wanted_resolution.x, wanted_resolution.y);
                __state.overlay_framebuffer = LoadRenderTexture(wanted_resolution.x * UI_SCALE, wanted_resolution.y * UI_SCALE);

                editor->f1_lock = true;
            }
        }
    }

    if (editor->should_playback) {
        bool flag = false;

#ifndef TARGET_ANDROID
        _fTextDraw(&__state.text_manager, "f1 to exit", {2, 2}, BLUE, 1);
#else
        flag = _fButtonDrawSimple("Exit", (IVector2){4, 4}, WHITE);
#endif

        if ((IsKeyPressed(KEY_F1) && !editor->f1_lock) || flag) {
            Vector2 dpi = GetWindowScaleDPI();

            editor->should_display_sidebar = true;
            editor->should_process_interactions = true;
            editor->should_playback = false;
            editor->level.entities = 0;

            UnloadRenderTexture(__state.framebuffer);
            UnloadRenderTexture(__state.overlay_framebuffer);

            __state.framebuffer = LoadRenderTexture((800 + 255) / UI_SCALE, 600 / UI_SCALE);
            __state.overlay_framebuffer = LoadRenderTexture(800 + 255, 600);

            for (fentity* e : editor->entities) {
                MemFree(e);
            }
            editor->entities.clear();
        }
        
        editor->f1_lock = false;
    }

    if (editor->should_display_selector) {
        IVector2 wsz = {
            __state.framebuffer.texture.width,
            __state.framebuffer.texture.height
        };

        Color c = BLACK;
        c.a = 128;

        DrawRectangle(0, 0, wsz.x, wsz.y, c);
    }

    if (editor->should_display_selector) {
        _fScheduleOverlayFunc([editor, bouncing_color](Vector2 mpos) {
            constexpr float offset_x = 50;
            constexpr float offset_y = 50;

            RLRectangle BG = { offset_x, offset_y, __state.overlay_framebuffer.texture.width - (offset_x * 2), __state.overlay_framebuffer.texture.height - (offset_y * 2) };
            RLRectangle blocks_check = { 0 };
            RLRectangle back = {60, 60, 50, 50};

            int blocks_count = 0;
            int layer = 1;
            int x_pos = 0;

            int mouse_button_down = 0;

            IVector2 cur_blocks_pos = { 0, 115 };
            IVector2 btn_tile_offset = {29, 6};

            constexpr float label_scaling = 5;
            float center_x = (BG.width - (editor->select_block_label.width * label_scaling)) / 2;
            
            DrawRectangle(BG.x, BG.y, BG.width, BG.height, { 50, 50, 50, 200 });
            DrawTextureEx(editor->select_block_label, { center_x + BG.x, BG.y + 15.f }, 0, label_scaling, WHITE);
            DrawRectangle(mpos.x, mpos.y, 10, 10, MAROON);
            if(CheckCollisionPointRec(mpos, back)) {
                btn_tile_offset.x = 31;
                if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
                    btn_tile_offset.x = 33;    
                    mouse_button_down = 5;
                }
                if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
                    editor->should_display_selector = ~editor->should_display_selector;            
                }
            }
            _fTilemapDrawMegatileScaled(editor->level.tilemap, {(int)back.x - 15, (int)back.y - 15}, btn_tile_offset, {2, 2}, 0, 0, WHITE, 5);
            _fTilemapDrawScaled(editor->level.tilemap, {(int)back.x + 5, (int)back.y + 5 - mouse_button_down}, {28, 6}, 0, 0, WHITE, 5);  
            for (int i = 0; i < editor->block_listing.total; i++) {
                fblock block = editor->block_listing.blocks[i];
                if (block.parent_id == 0) {
                    if (blocks_count > 0) {
                        cur_blocks_pos.x += 100;
                    }
                    blocks_count++;

                    if (cur_blocks_pos.x >= BG.width) {
                        cur_blocks_pos.y += 100;
                        cur_blocks_pos.x = 100;
                    }
                    blocks_check = { (float)cur_blocks_pos.x,(float)cur_blocks_pos.y,(float)editor->level.tilemap->tile_size.x * 5, (float)editor->level.tilemap->tile_size.x * 5 };

                    _fTilemapDrawScaled(editor->level.tilemap, cur_blocks_pos, { block.base.tile_x, block.base.tile_y }, 0, 0, WHITE, 5);
                    if (CheckCollisionPointRec(mpos, blocks_check)) {
                        DrawRectangleRec(blocks_check, bouncing_color);
                        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                            editor->current_block_id = i;
                            editor->should_display_selector = ~editor->should_display_selector;          
                        }
                    }
                }
            }

            _fSliderDraw(&editor->test_slider);
        });
    }
}
