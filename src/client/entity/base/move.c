#include <fightable/entity.h>

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