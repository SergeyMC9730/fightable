#include "raylib.h"
#include <raymath.h>
#include <cmath>
#include <hitbox.h>
#include <stdint.h>
#include <algorithm>
#include <vector>
Camera2D camera;

float m_speedX = 0.0f;
float m_speedY = 0.0f;
bool m_onGround;
struct Bullet {
     Vector2 position;
     float rotation;
     float speed;
     float startTime;
};
Vector2 convertToCameraPos(Vector2 pos) {
    return Vector2 {
        (camera.target.x - camera.offset.x / camera.zoom) + pos.x / camera.zoom, 
        (camera.target.y - camera.offset.y / camera.zoom) + pos.y / camera.zoom
    };
}
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
int main(void) {
    const int screenWidth = 800;
    const int screenHeight = 600;

    InitWindow(screenWidth, screenHeight, "Shoothing System");

    SetTargetFPS(60);

    RLRectangle gun = {0, 0, 25, 100};
    fhitbox player = {10, 75, 50, 50};
    fhitbox placement[] = {
        {-100, 500, 2000, 100},
        {0, 0, 720, 8},
    };

    int side = 0;
    int bulletStartPos;
    int bulletStartTime;
    
    bool bulletActive = false;
    bool debug = 0;
    bool isCanJump = 1;
    bool bulletLock = 0;

    const float g = 9.8;
    float rad;
    float delay = 0;
    float v = 3;
    float playerRotate = 1.0f;

    Color bouncingColor = RED;

    Bullet bullet;
    bullet.speed = 5;
    
    Texture2D playerTEX = LoadTexture("assets/guntest/player.png");
    Texture2D bigGun = LoadTexture("assets/guntest/pistolet.png");

    uint16_t ammo = 16;
    std::vector<Bullet> bullets {

    };
    float weaponRot;
    float playerWeaponRot;
    while (!WindowShouldClose())  {
        bouncingColor.r++;
        bouncingColor.g++;
        bouncingColor.b++;  
        if(delay != 0) {
            delay -= 0.5;
        }

        Vector2 mpos = convertToCameraPos(GetMousePosition());
        Vector2 delta;
        delta.x = mpos.x - player.x - (player.width / 2);
        delta.y = mpos.y - player.y - (player.height / 2);

        float time = GetTime() / 100;

        camera.target = {player.x, 0};
        camera.offset = (Vector2){(float)GetScreenWidth() / 2, 0};
        camera.rotation = 0.0f;
        camera.zoom = 1.0f;

        
        weaponRot = atan2(mpos.y - player.y, mpos.x - (player.x + player.width / 2)) * RAD2DEG;
        if(mpos.x < camera.target.x) {
            playerRotate = -1.0f;
            playerWeaponRot = -95;
        } 
        if(mpos.x > camera.target.x) {
            playerRotate = 1.0f;
            playerWeaponRot = 0;
        }

        for(int i = 0; i < bullets.size(); i++) {
            bullets[i].position.x += bullets[i].speed * cos(bullets[i].rotation);
            bullets[i].position.y += bullets[i].speed * sin(bullets[i].rotation);
            if(GetTime() - bullets[i].startTime >= 5) {
                bullets.erase(bullets.begin() + i);
            }
        }

        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT) && ammo != 0 && delay == 0) {
            delay = 10;
            bullet.startTime = GetTime();
            bullet.rotation = atan2(delta.y, delta.x);
            bullet.position = (Vector2){player.x + player.width / 2, player.y + player.width / 2};
            bullets.push_back(bullet);
            if(ammo > 0) {
                ammo--;
            }
        }
        if(IsKeyDown(KEY_R)) {
            ammo = 16;
        }
        if(IsKeyPressed(KEY_F3)) {
            debug = !debug;
        }
        gun = {(player.x + player.width), (player.y + player.height / 2)};
        if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) m_speedX += 1.1 * 1.1;
        if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A)) m_speedX -= 1.1 * 1.1;
        if ((IsKeyDown(KEY_W) || IsKeyDown(KEY_SPACE)) && m_onGround) {
            m_speedY = -15;
        }
        _fMovePlayer(m_speedX, m_speedY, placement, &player);
        m_speedY += 1;
        m_speedX *= 0.850f;
        m_speedY *= 0.850f; 
        if(m_onGround) {   

        }
        BeginDrawing();
            ClearBackground(RAYWHITE);

            BeginMode2D(camera);        

            DrawRectangle(placement[0].x, placement[0].y, placement[0].width, placement[0].height, MAROON);
            DrawRectangle(placement[1].x, placement[1].y, placement[1].width, placement[1].height, MAROON);
            DrawRectangle(player.x, player.y, player.width, player.height, BLUE);
            DrawTexturePro(playerTEX, {0, 0, 8 * playerRotate, 8}, {player.x, player.y, 50, 50}, {0, 0}, 0, RAYWHITE);
            DrawTexturePro(bigGun, {0, 0, 11, 8 * playerRotate}, {player.x + player.width + 20 + playerWeaponRot, player.y + 25, 50, 40}, {50 / 2, 40 / 2}, weaponRot, RAYWHITE);            
            DrawLine(player.x + player.width, player.y, mpos.x, mpos.y, GREEN);
            for(int i = 0; i < bullets.size(); i++) {
                DrawCircle(bullets[i].position.x, bullets[i].position.y, 5, GOLD);
            }
            if(debug) {
                RlDrawText("Debug Mode", 0, 0, 25, bouncingColor);
                DrawFPS(0, 25);
                if(bullets.size() > 0) {
                    RlDrawText(TextFormat("Bullet X: %f", bullets[0].position.x), 0, 50, 25, MAROON);
                    RlDrawText(TextFormat("Bullet Y: %f", bullets[0].position.y), 0, 75, 25, MAROON);
                    RlDrawText(TextFormat("%f", bullets[0].startTime), 0, 175, 25, MAROON);
                    RlDrawText(TextFormat("Delay: %f", delay), 0, 200, 25, MAROON);
                    RlDrawText(TextFormat("Bullet size: %i", bullets.size()), 0, 225, 25, MAROON);
                }
                RlDrawText(TextFormat("Radium: %f", rad), 0, 100, 25, MAROON);
                RlDrawText(TextFormat("Rotation: %f", bullet.rotation), 0, 125, 25, MAROON);
                RlDrawText(TextFormat("Delta: %f, %f", delta.x, delta.y), 0, 150, 25, MAROON);

            }
            
            EndMode2D();
        EndDrawing();
    }
    RlCloseWindow();

    return 0;
}

