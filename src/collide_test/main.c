#include <raylib.h>
#include <hitbox.h>
#include <stdlib.h>

struct fentity {
    Vector2 speed;

    unsigned char on_ground : 1;
    unsigned char moving_horizontally : 1;
    unsigned char jumping : 1;
    unsigned char moving_negative : 1;
    unsigned char complete_px : 1;
    unsigned char complete_nx : 1;

    fhitbox hitbox;

    fhitbox *obstacles;
    unsigned int obstacles_length;

    fhitbox ground_hitbox;
};

void _fEntityMove(struct fentity *entity, Vector2 pos) {
    float delta = GetFrameTime();
    pos.y *= delta;
    pos.x *= delta;

    float prevX = pos.x;
    float prevY = pos.y;

    if (prevX != 0.f) {
        // Check for X collision
        for (unsigned int i = 0; i < entity->obstacles_length; i++) {
            pos.x = _fHitboxClipXCollide(entity->obstacles + i, &entity->hitbox, pos.x);
        }

        entity->hitbox.x += pos.x;

        // Stop motion on collision
        if (prevX != pos.x) entity->speed.x = 0.f;
    }

    if (prevY != 0.f) {
        for (unsigned int i = 0; i < entity->obstacles_length; i++) {
            pos.y = _fHitboxClipYCollide(entity->obstacles + i, &entity->hitbox, pos.y);
        }

        entity->hitbox.y += pos.y;

        // Stop motion on collision
        if (prevY != pos.y) entity->speed.y = 0.f;  
    }
}

void _fEntityUpdate(struct fentity *entity) {
    static const double max_speed_x = 1.5 / 1.1 * 70.f;
    static const double max_speed_y = 3.f * 25.f;

    const double delta = GetFrameTime();

    if (entity->moving_horizontally && !entity->moving_negative) {
        entity->speed.x += delta * 180.f;
        if (entity->speed.x > max_speed_x) {
            entity->speed.x = max_speed_x;
        }
        entity->complete_px = 1;
    } 
    if (entity->moving_horizontally && entity->moving_negative) {
        entity->speed.x -= delta * 180.f;
        if (entity->speed.x < -max_speed_x) {
            entity->speed.x = -max_speed_x;
        }
        entity->complete_nx = 1;
    }
    else if (!entity->moving_horizontally) {
        if (entity->complete_nx) {
            entity->speed.x += delta * 350.f;

            if (entity->speed.x > 0) {
                entity->speed.x = 0;
                entity->complete_nx = 0;
            }
        }
        if (entity->complete_px) {
            entity->speed.x -= delta * 350.f;

            if (entity->speed.x < 0) {
                entity->speed.x = 0;
                entity->complete_px = 0;
            }
        }
    }

    if (!entity->on_ground) {
        entity->speed.y += delta * 100.f;
        if (entity->speed.y > max_speed_y) {
            entity->speed.y = max_speed_y;
        }

        float s = entity->speed.y;

        if (entity->speed.y > 0.f) {
            s *= 1.25f;
        }

        _fEntityMove(entity, (Vector2){0, s * 3.f});
    }

    _fEntityMove(entity, entity->speed);

    entity->ground_hitbox.width = entity->hitbox.width;
    entity->ground_hitbox.height = 1;
    entity->ground_hitbox.x = entity->hitbox.x;
    entity->ground_hitbox.y = entity->hitbox.y + (entity->hitbox.height - entity->ground_hitbox.height) + 1;

    RLRectangle r1 = *(RLRectangle*)(&entity->ground_hitbox);

    entity->on_ground = 0;

    for (unsigned int i = 0; i < entity->obstacles_length; i++) {
        RLRectangle r2 = *(RLRectangle*)(entity->obstacles + i);

        if (CheckCollisionRecs(r1, r2)) {
            entity->on_ground = 1;
            break;
        }
    }
}
void _fEntityInit(struct fentity* entity) {

}
void _fEntityJump(struct fentity* entity) {
     if (entity->on_ground) {
        entity->speed.y = -32.5f;
     }
}

int main() {
    InitWindow(800, 600, "Collide Test");

    SetTargetFPS(60);

    fhitbox player = {0, -128, 8, 8};
    fhitbox placement[] = {
        {.width = 8, .height = 32, .x = 60},
        {.width = 8, .height = 8, .x = 0, .y = 64},
        {.width = 8, .height = 8, .x = 8, .y = 64},
        {.width = 8, .height = 8, .x = 16, .y = 64},
        {.width = 8, .height = 8, .x = 32, .y = 64},
        {.width = 128, .height = 8, .x = 40, .y = 48},
        {.width = 8, .height = 32, .x = 40, .y = 0},
    };

    struct fentity test_entity = { 0 };
    _fEntityInit(&test_entity);

    test_entity.hitbox = player;
    test_entity.obstacles = placement;
    test_entity.obstacles_length = sizeof(placement) / sizeof(fhitbox);

    char* buf = (char*)MemAlloc(512);

    RenderTexture2D txt = LoadRenderTexture(800 / 5, 600 / 5);

    Camera2D cam = { 0 };
    cam.zoom = 1.f;

    while(!WindowShouldClose()) {
        double deltaTime = GetFrameTime();

        test_entity.moving_horizontally = 0;
        test_entity.moving_negative = 0;

        if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) {
            test_entity.moving_horizontally = 1;
            test_entity.moving_negative = 0;
        }
        if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A)) {
            test_entity.moving_horizontally = 1;
            test_entity.moving_negative = 1;
        }
        if ((IsKeyDown(KEY_W) || IsKeyDown(KEY_SPACE))) {
            _fEntityJump(&test_entity);
        }

        _fEntityUpdate(&test_entity);

        snprintf(buf, 512, "on_ground: %d\nspeed: {%ff, %ff}\npos: {%ff, %ff}\n%d%d%d%d%d%d%d",
            test_entity.on_ground,
            test_entity.speed.x, test_entity.speed.y,
            test_entity.hitbox.x, test_entity.hitbox.y,
            test_entity.on_ground, 0, test_entity.moving_horizontally,
            test_entity.jumping, test_entity.moving_negative, test_entity.complete_px,
            test_entity.complete_px
        );

        BeginDrawing();
        BeginTextureMode(txt);
        BeginMode2D(cam);

        ClearBackground(RAYWHITE);

        DrawRectangle(0, GetRenderHeight() - 24, GetRenderWidth(), 24, RED);
        
        fhitbox h = test_entity.hitbox;
        RLRectangle r = { h.x, h.y, h.width, h.height };

        DrawRectangleRec(r, GREEN);

        for (int i = 0; i < test_entity.obstacles_length; i++) {
            h = test_entity.obstacles[i];
            r = (RLRectangle){ h.x, h.y, h.width, h.height };

            DrawRectangleRec(r, RED);
        }

        EndMode2D();
        EndTextureMode();

        RLRectangle source = (RLRectangle){ 0, 0, (double)txt.texture.width, (double)-txt.texture.height };
        RLRectangle dest = (RLRectangle){ 0, 0, 800, 600 };

        DrawTexturePro(txt.texture, source, dest, (Vector2) { 0, 0 }, 0.f, WHITE);

        RlDrawText(buf, 2, 2, 20, BLUE);

        EndDrawing();
    }
    RlCloseWindow();
    return 0;
}