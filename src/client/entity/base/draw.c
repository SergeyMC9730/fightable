#include <fightable/entity.h>

void _fEntityDraw(struct fentity *entity) {
    if (!entity) return;

    Rectangle hitbox = entity->hitbox;
    if (entity->standing) {
        hitbox.y -= 1;
    }

    DrawRectangleRec(hitbox, BLUE);

    if (entity->debug_draw) {
        // DrawRectangleRec(entity->hitbox_r, RED);
        // DrawRectangleRec(entity->hitbox_l, GREEN);
        // DrawRectangleRec(entity->hitbox_u, YELLOW);
        // DrawRectangleRec(entity->hitbox_d, PURPLE);

        DrawRectangleRec(entity->collided_surface, (Color){255, 64, 64, 64});
        DrawRectangleRec(entity->standing_surface, (Color){64, 255, 64, 64});
        DrawRectangleRec(entity->ceiling_surface, (Color){64, 64, 255, 64});
    }
}