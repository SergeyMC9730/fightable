#include <raylib.h>


#define X_SPEED 70.f
#define Y_SPEED 150.f

struct fentity {
    double time1;

    double accel_x;
    double accel_y;
    
    unsigned char jumping : 1;
    unsigned char walking : 1;
    unsigned char negative_move : 1;
    unsigned char falling : 1;

    unsigned char touching_wall_l : 1;
    unsigned char touching_wall_r : 1;
    unsigned char touching_ceiling : 1;
    unsigned char standing : 1;

    RLRectangle hitbox;
    RLRectangle *obstacles;
    unsigned int obstacles_length;

    RLRectangle hitbox_r;
    RLRectangle hitbox_l;
    RLRectangle hitbox_u;
    RLRectangle hitbox_d;

    RLRectangle standing_surface;
    RLRectangle ceiling_surface;

    unsigned char ccheck0 : 1;
    unsigned char ccheck1 : 1;

    unsigned int frames_since_jump;
};

#include <stdio.h>

RLRectangle to_int_rect(RLRectangle r) {
    RLRectangle _r = {.x = (int)r.x, .y = (int)r.y, .width = (int)r.width, .height = (int)r.height};

    return _r;
}

void update_entity(struct fentity *entity) {
    // printf("%f %f %f; l%d,r%d,c%d,s%d\n", entity->accel_x, entity->hitbox.x, entity->hitbox.y, entity->touching_wall_l, entity->touching_wall_r, entity->touching_ceiling, entity->standing);

    double delta = (double)GetFrameTime();

    static const double x_speed = X_SPEED;
    static const double x_stop_speed = 3.f;

    static const double max_accel_x = X_SPEED;
    static const double min_accel_x = 0.f;

    double flag = 1.f;

    entity->touching_ceiling = 0;
    entity->touching_wall_l = 0;
    entity->touching_wall_r = 0;
    entity->standing = 0;
    entity->standing_surface = (RLRectangle){ 0 };
    entity->ceiling_surface = (RLRectangle){ 0 };

    for (int i = 0; i < entity->obstacles_length; i++) {
        RLRectangle r = entity->obstacles[i];

        unsigned char checks[4] = {
            CheckCollisionRecs(entity->hitbox_l, r),
            CheckCollisionRecs(entity->hitbox_r, r),
            CheckCollisionRecs(entity->hitbox_u, r),
            CheckCollisionRecs(entity->hitbox_d, r)
        };

        if (checks[2]) {
            entity->touching_ceiling = 1;
            entity->ceiling_surface = r;
            entity->hitbox.y = entity->ceiling_surface.y + (entity->ceiling_surface.height + 1);
            entity->accel_y = 0;

            entity->hitbox_r = (RLRectangle){.width = 1, .height = entity->hitbox.height - 1, .x = entity->hitbox.x + (entity->hitbox.width - 1), .y = entity->hitbox.y};
            entity->hitbox_l = (RLRectangle){.width = 1, .height = entity->hitbox.height - 1, .x = entity->hitbox.x, .y = entity->hitbox.y};
        
            checks[0] = CheckCollisionRecs(entity->hitbox_l, r);
            checks[1] = CheckCollisionRecs(entity->hitbox_r, r);
        }

        if (checks[0] && !checks[3]) {
            entity->touching_wall_l = 1;
        }
        if (checks[1] && !checks[3]) {
            entity->touching_wall_r = 1;
        }
        if (checks[3]) {
            entity->standing = 1;
            entity->standing_surface = r;
            entity->frames_since_jump = 0;
        }
    }

    unsigned char jincreased = 0; 

    if (entity->frames_since_jump > 0 || entity->accel_y < 0) {
        entity->frames_since_jump++;
    }

    if (entity->walking) {
        if (entity->negative_move) {
            flag = -flag;
        }

        entity->accel_x += delta * x_speed * flag * x_stop_speed;

        if (entity->accel_x > max_accel_x) {
            entity->accel_x = max_accel_x;
        }
        if (entity->accel_x < -max_accel_x) {
            entity->accel_x = -max_accel_x;
        }
    } else {
        if (entity->negative_move) {
            entity->accel_x += delta * x_speed * x_stop_speed;

            if (entity->accel_x > 0) {
                entity->accel_x = 0;
            } 
        } else {
            entity->accel_x -= delta * x_speed * x_stop_speed;

            if (entity->accel_x < 0) {
                entity->accel_x = 0;
            } 
        }
    }

    static const double y_speed = Y_SPEED;
    static const double max_accel_y = Y_SPEED;

    // if (entity->accel_y < 0 && entity->jumping) {
    //     entity->ccheck0 = 1;
    // } else {
    //     entity->ccheck0 = 0;
    // }

    if (!entity->standing || entity->jumping || entity->touching_ceiling) {
        entity->accel_y += y_speed * delta * 3;

        if (entity->accel_y > max_accel_y) {
            entity->accel_y = max_accel_y;
        }
    } else {
        entity->accel_y = 0;
        //if (entity->touching_wall_l && entity->touching_wall_r) {
            entity->hitbox.y = entity->standing_surface.y - (entity->hitbox.height - 1);
        //}
    }

    if (entity->touching_wall_l && entity->accel_x < 0) {
        entity->accel_x = 0;
    }
    if (entity->touching_wall_r && entity->accel_x > 0) {
        entity->accel_x = 0;
    }

    entity->hitbox.x += entity->accel_x * delta;
    entity->hitbox.y += entity->accel_y * delta;

    entity->hitbox_r = (RLRectangle){.width = 1, .height = entity->hitbox.height - 1, .x = entity->hitbox.x + (entity->hitbox.width - 1), .y = entity->hitbox.y};
    entity->hitbox_l = (RLRectangle){.width = 1, .height = entity->hitbox.height - 1, .x = entity->hitbox.x, .y = entity->hitbox.y};
    entity->hitbox_u = (RLRectangle){.width = entity->hitbox.width - 2, .height = 1, .x = entity->hitbox.x + 1, .y = entity->hitbox.y};
    entity->hitbox_d = (RLRectangle){.width = entity->hitbox.width - 2, .height = 1, .x = entity->hitbox.x + 1, .y = entity->hitbox.y + (entity->hitbox.height - 1)};
}

void draw_entity(struct fentity *entity) {
    RLRectangle hitbox = entity->hitbox;
    if (entity->standing) {
        hitbox.y -= 1;
    }

    DrawRectangleRec(hitbox, BLUE);

    DrawRectangleRec(entity->hitbox_r, RED);
    DrawRectangleRec(entity->hitbox_l, GREEN);
    DrawRectangleRec(entity->hitbox_u, YELLOW);
    DrawRectangleRec(entity->hitbox_d, PURPLE);
}

int main() {
    char *buf = (char *)MemAlloc(512);

    printf("Preinit!\n");

    InitWindow(800, 600, "Physics Test");

    SetTargetFPS(60);

    struct fentity player = {.hitbox = {.width = 8, .height = 8}};
    RenderTexture2D txt = LoadRenderTexture(800 / 5, 600 / 5);

    RLRectangle obstacles[] = {
        {.width = 8, .height = 32, .x = 60},
        {.width = 8, .height = 8, .x = 0, .y = 64},
        {.width = 8, .height = 8, .x = 8, .y = 64},
        {.width = 8, .height = 8, .x = 16, .y = 64},
        {.width = 8, .height = 8, .x = 32, .y = 64},
        {.width = 128, .height = 8, .x = 40, .y = 64},
        {.width = 8, .height = 64, .x = 40, .y = 0},
    };

    player.obstacles = obstacles;
    player.obstacles_length = sizeof(obstacles) / sizeof(RLRectangle);

    Camera2D cam = { 0 };
    cam.zoom = 1.f;

    while (!WindowShouldClose()) {
        player.walking = 0;
        player.jumping = 0;

        if (IsKeyDown(KEY_A)) {
            player.walking = 1;
            player.negative_move = 1;
        } else if (IsKeyDown(KEY_D)) {
            player.walking = 1;
            player.negative_move = 0;
        }

        if (IsKeyPressed(KEY_SPACE)) {
            player.jumping = 1;
            player.accel_y = -138;
        }

        if (IsKeyDown(KEY_UP)) {
            cam.target.y -= GetFrameTime() * 20.f;
        }
        if (IsKeyDown(KEY_DOWN)) {
            cam.target.y += GetFrameTime() * 20.f;
        }

        update_entity(&player);

        snprintf(buf, 512, "ax:%d\nay:%d\n%f:%f\nSF: %d:%d:%d:%d\nl%d,r%d,c%d,s%d\nCF: %d:%d:%d:%d\nj%d,c%d,C%d\njf%d",
            (int)player.accel_x, (int)player.accel_y,
            player.hitbox.x, player.hitbox.y,
            (int)player.standing_surface.x, (int)player.standing_surface.y, (int)player.standing_surface.width, (int)player.standing_surface.height,
            player.touching_wall_l, player.touching_wall_r, player.touching_ceiling, player.standing,
            (int)player.ceiling_surface.x, (int)player.ceiling_surface.y, (int)player.ceiling_surface.width, (int)player.ceiling_surface.height,
            player.jumping, player.ccheck0, player.ccheck1,
            player.frames_since_jump
        );

        BeginDrawing();
        BeginTextureMode(txt);
        BeginMode2D(cam);

        ClearBackground(RAYWHITE);

        DrawRectangle(0, GetRenderHeight() - 24, GetRenderWidth(), 24, RED);
        draw_entity(&player);

        for (int i = 0; i < player.obstacles_length; i++) {
            RLRectangle r = player.obstacles[i];

            DrawRectangleRec(r, RED);
        }

        EndMode2D();
        EndTextureMode();

        RLRectangle source = (RLRectangle){ 0, 0, (float)txt.texture.width, (float)-txt.texture.height };
        RLRectangle dest = (RLRectangle){ 0, 0, 800, 600 };

        DrawTexturePro(txt.texture, source, dest, (Vector2){0, 0}, 0.f, WHITE);

        RlDrawText(buf, 2, 2, 10, RED);

        EndDrawing();
    }

    RlCloseWindow();

    return 0;
}