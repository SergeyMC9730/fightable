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
enum Weapons {
    GUN = 0,
    REVOLVER,
    SHOTGUN,
    ROCKETLAUNCHER
};
struct Bullet {
     Vector2 position;
     float rotation;
     float speed;
     float startTime;
     Vector2 posRelToPlr;
};
struct Weapon {
    // TODO: switch Texture2D to Vector2 posInTile
    Texture2D weapon;
    Bullet bullet;
    // weaponDulo - это расстояние от центра оружие до самого дула, центр оружия просчитывается в функции отрисовки
    Vector3 weaponDulo;
    float weaponRotation;
    float weaponPositionRot;
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

    fhitbox player = {10, 75, 50, 50};
    fhitbox placement[] = {
        {-100, 500, 2000, 100},
        {0, 0, 720, 8},
    };
    
    bool debug = 0;
    bool isCanJump = 1;

    float delay = 0;
    float playerRotate = 1.0f;
    float playerWeaponRot;
    float lineRotate = 25;
    
    Color bouncingColor = RED;
    
    Texture2D playerTEX = LoadTexture("assets/guntest/player.png");
    Texture2D bigGun = LoadTexture("assets/guntest/pistolet.png");
    Texture2D revolver = LoadTexture("assets/guntest/revolver.png");
    Texture2D shotgun = LoadTexture("assets/guntest/shotgun.png");
    Texture2D rocketLauncher = LoadTexture("assets/guntest/rocketlauncher.png");

    uint16_t ammo = 16;
    std::vector<Bullet> bullets {

    };
    std::vector<Weapon> weapons {
        {},
        {bigGun, {0, 0, 5, 0}, {25, 15, -25}, 0},
        {revolver, {0, 0, 5, 0}, {0, 0, 0}, 0},
        {shotgun, {0, 0, 5, 0}, {0, 0, 0}, 0},
        {rocketLauncher, {0, 0, 5, 0}, {0, 0, 0}, 0}
    };
    
    Weapon currentWeapon;
    currentWeapon = weapons[1];
    currentWeapon.bullet.speed = 5;
    float currentWeaponPos;
    while (!WindowShouldClose())  {
        bouncingColor.r++;
        bouncingColor.g++;
        bouncingColor.b++;  
        if(delay != 0) {
            delay -= 0.5;
        }
        for(int i = 0; i < weapons.size(); i++) {
            if(IsKeyDown(KEY_ONE + i)) {
                currentWeapon = weapons[i + 1];
            }
        }
        Vector2 mpos = convertToCameraPos(GetMousePosition());

        Vector2 delta;
        delta.x = mpos.x - player.x - (player.width / 2);
        delta.y = mpos.y - player.y - (player.height / 2);

        camera.target = {player.x, 0};
        camera.offset = (Vector2){(float)GetScreenWidth() / 2, 0};
        camera.rotation = 0.0f;
        camera.zoom = 1.0f;

        currentWeapon.weaponRotation= atan2(mpos.y - player.y - 15, mpos.x - (player.x + player.width / 2 - 10));
        if(mpos.x < camera.target.x + player.width / 2) {
            playerRotate = -1.0f;
            playerWeaponRot = -95;
            currentWeaponPos = currentWeapon.weaponDulo.z;
        } 
        if(mpos.x > camera.target.x + player.width / 2) {
            playerRotate = 1.0f;
            playerWeaponRot = 0;
            currentWeaponPos = currentWeapon.weaponDulo.x;
        }

        for(int i = 0; i < bullets.size(); i++) {
            if(bullets[i].posRelToPlr.x < bullets[i].posRelToPlr.y) {
                bullets[i].position.x -= bullets[i].speed;
            } 
            if(bullets[i].posRelToPlr.x > bullets[i].posRelToPlr.y) {
                bullets[i].position.x += bullets[i].speed;
            }
 //  * cos(bullets[i].rotation)
            // bullets[i].position.y += bullets[i].speed * sin(bullets[i].rotation); 
            if(GetTime() - bullets[i].startTime >= 5) {
                bullets.erase(bullets.begin() + i);
            }
        }

        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT) && ammo != 0 && delay == 0) {
            delay = 10;
            currentWeapon.bullet.startTime = GetTime();
            currentWeapon.bullet.rotation = atan2(delta.y, delta.x);
            currentWeapon.bullet.position = (Vector2){player.x + player.width + 20 + playerWeaponRot + currentWeaponPos, player.y + currentWeapon.weaponDulo.y};
            currentWeapon.bullet.posRelToPlr.x = mpos.x;
            currentWeapon.bullet.posRelToPlr.y = camera.target.x + player.width / 2;
            bullets.push_back(currentWeapon.bullet);
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
        float xd = mpos.x - (player.x + player.width + 20 + playerWeaponRot) - (currentWeapon.weapon.width * 5 / 2);
        float yd = mpos.y - (player.y + 25) - (currentWeapon.weapon.height * 5 / 2);
        float rot = atan2(xd, yd);
        BeginDrawing();
            ClearBackground(RAYWHITE);

            BeginMode2D(camera);        

            DrawRectangle(placement[0].x, placement[0].y, placement[0].width, placement[0].height, MAROON);
            DrawRectangle(placement[1].x, placement[1].y, placement[1].width, placement[1].height, MAROON);
            DrawRectangle(player.x, player.y, player.width, player.height, BLUE);
            
            DrawTexturePro(playerTEX, {0, 0, 8 * playerRotate, 8}, {player.x, player.y, 50, 50}, {0, 0}, 0, RAYWHITE);    
            DrawTexturePro(currentWeapon.weapon, {0, 0, (float)currentWeapon.weapon.width, (float)currentWeapon.weapon.height * playerRotate}, {player.x + player.width + 20 + playerWeaponRot, player.y + 25, (float)currentWeapon.weapon.width * 5, (float)currentWeapon.weapon.height * 5}, {50 / 2, 40 / 2}, currentWeapon.weaponRotation * RAD2DEG, RAYWHITE);   //         
            
            DrawLine((player.x + player.width + 20 + playerWeaponRot) + (currentWeapon.weapon.width * 5 / 2) * cos(rot), player.y + ((float)currentWeapon.weapon.width * 5) * sin(rot), mpos.x, mpos.y, GREEN); //   
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
                RlDrawText(TextFormat("Rotation: %f", currentWeapon.bullet.rotation), 0, 125, 25, MAROON);
                RlDrawText(TextFormat("Delta: %f, %f", delta.x, delta.y), 0, 150, 25, MAROON);
                RlDrawText(TextFormat("Bullet Speed: %f", weapons[0].bullet.speed), 0, 250, 25, MAROON);
            }
            
            EndMode2D();
        EndDrawing();
    }
    RlCloseWindow();

    return 0;
}