#include <raylib.h>
#include <hitbox.h>
#include <stdlib.h>
float m_speedX = 0.0f;
float m_speedY = 0.0f;
bool m_onGround;

void _fMovePlayer(float x, float y, struct fhitbox* room, struct fhitbox* player) {
    float prevX = x;
    float prevY = y;
    bool isTouch = 0;
    // Check for X collision
    for (int i = 0; i < 2; i++) {
        x = _fHitboxClipXCollide(room + i, player, x);
    }
    player->x += x;
    // printf("%f \n", x);
    // Check for Y collision
    for (int i = 0; i < 2; i++) {
        y = _fHitboxClipYCollide(room + i, player, y);
    }

    player->y += y;
    // printf("%f \n", y);

    m_onGround = prevY != y && prevY > 0.f;
    // Stop motion on collision
    if (prevX != x) m_speedX = 0.f;
    if (prevY != y) m_speedY = 0.f;
} 
int main() {
    InitWindow(800, 600, "Collide Test");

    SetTargetFPS(144);

    fhitbox player = {0, 200, 50, 100};
    fhitbox placement[] = {
        {0, 500, 720, 100},
        {0, 0, 720, 8},
    };

    bool isCanJump = 1;

    while(!WindowShouldClose()) {
        float deltaTime = GetFrameTime();
        if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) m_speedX += 60 * deltaTime;
        if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A)) m_speedX -= 60 * deltaTime;
        if ((IsKeyDown(KEY_W) || IsKeyDown(KEY_SPACE)) && m_onGround) {
            m_speedY = -3;
        }
        _fMovePlayer(m_speedX, m_speedY, placement, &player);

        m_speedY += 3 * deltaTime;

        m_speedX *= 0.7f;
        m_speedY *= 0.98f;
        
        if(m_onGround) {
            m_speedX *= 0.7f;
            m_speedY *= 0.80f;
        }
        
        BeginDrawing();
            ClearBackground(RAYWHITE);

            DrawRectangle(placement[0].x, placement[0].y, placement[0].width, placement[0].height, MAROON);
            DrawRectangle(placement[1].x, placement[1].y, placement[1].width, placement[1].height, MAROON);
            DrawRectangle(player.x, player.y, player.width, player.height, MAROON);

            DrawText("Congrats! You created your first window!", 190, 200, 20, LIGHTGRAY);

            DrawFPS(0, 0);

        EndDrawing();
    }
    CloseWindow();
    return 0;
}