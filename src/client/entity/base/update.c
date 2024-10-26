#include <fightable/entity.h>
#include <raylib.h>
#include <stdio.h>
#include <fightable/debug.h>

// #define DEBUG

void _fEntityUpdate(struct fentity *entity) {
    if (!entity || entity->no_ai) return;

    // printf("%f %f %f; l%d,r%d,c%d,s%d\n", entity->accel_x, entity->hitbox.x, entity->hitbox.y, entity->touching_wall_l, entity->touching_wall_r, entity->touching_ceiling, entity->standing);

    double delta = entity->delta;

    static const double x_speed = 70.f;
    static const double x_stop_speed = 3.f;

    static const double max_accel_x = x_speed;
    static const double min_accel_x = 0.f;

    double flag = 1.f;

    entity->touching_ceiling = 0;
    entity->touching_wall_l = 0;
    entity->touching_wall_r = 0;
    entity->standing = 0;
    entity->standing_surface = (Rectangle){};
    entity->ceiling_surface = (Rectangle){};

    for (int i = 0; i < entity->obstacles_length; i++) {
        Rectangle r = entity->obstacles[i];

        if ((int)r.width <= 0 || (int)r.height <= 0) continue;

        unsigned char checks[4] = {
            CheckCollisionRecs(entity->hitbox_l, r),
            CheckCollisionRecs(entity->hitbox_r, r),
            CheckCollisionRecs(entity->hitbox_u, r),
            CheckCollisionRecs(entity->hitbox_d, r)
        };

         if (checks[0] && !checks[3]) {
            entity->touching_wall_l = 1;
            entity->collided_surface = r;
        }
        if (checks[1] && !checks[3]) {
            entity->touching_wall_r = 1;
            entity->collided_surface = r;
        }

        if (checks[2] && !entity->touching_wall_r && !entity->touching_wall_l) {
            entity->touching_ceiling = 1;
            entity->ceiling_surface = r;
            entity->hitbox.y = entity->ceiling_surface.y + (entity->ceiling_surface.height + 1);
            entity->accel_y = 0;

            entity->hitbox_r = (Rectangle){.width = 1, .height = entity->hitbox.height - 1, .x = entity->hitbox.x + (entity->hitbox.width - 1), .y = entity->hitbox.y};
            entity->hitbox_l = (Rectangle){.width = 1, .height = entity->hitbox.height - 1, .x = entity->hitbox.x, .y = entity->hitbox.y};
        
            checks[0] = CheckCollisionRecs(entity->hitbox_l, r);
            checks[1] = CheckCollisionRecs(entity->hitbox_r, r);

#ifdef DEBUG
            printf("collision with ceiling %d:%d:%d:%d happened. resetting accel_y, updating collision flags\n", (int)r.x, (int)r.y, (int)r.width, (int)r.height);
#endif
        }

        if (checks[0] && !checks[3]) {
            entity->touching_wall_l = 1;
            entity->collided_surface = r;

#ifdef DEBUG
            printf("player is touching left wall %d:%d:%d:%d\n", (int)r.x, (int)r.y, (int)r.width, (int)r.height);
#endif
        }
        if (checks[1] && !checks[3]) {
            entity->touching_wall_r = 1;
            entity->collided_surface = r;

#ifdef DEBUG
            printf("player is touching right wall %d:%d:%d:%d\n", (int)r.x, (int)r.y, (int)r.width, (int)r.height);
#endif
        }
        if (checks[3]) {
            entity->standing = 1;
            entity->standing_surface = r;
            entity->frames_since_jump = 0;

#ifdef DEBUG
            printf("player is touching surface %d:%d:%d:%d\n", (int)r.x, (int)r.y, (int)r.width, (int)r.height);
#endif
        }
    }

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

    static const double y_speed = 150.f;
    static const double max_accel_y = y_speed;

    if (!entity->standing || entity->jumping || entity->touching_ceiling) {
        entity->accel_y += y_speed * delta * 3;

        if (entity->accel_y > max_accel_y) {
            entity->accel_y = max_accel_y;
        }
    } else {
        entity->accel_y = 0;
        entity->hitbox.y = entity->standing_surface.y - (entity->hitbox.height - 1);
    }

    if (entity->touching_wall_l && entity->accel_x < 0) {
        entity->accel_x = 0;
    }
    if (entity->touching_wall_r && entity->accel_x > 0) {
        entity->accel_x = 0;
    }

    entity->hitbox.x += entity->accel_x * delta;
    entity->hitbox.y += entity->accel_y * delta;

    entity->hitbox_r = (Rectangle){.width = 1, .height = entity->hitbox.height - 1, .x = entity->hitbox.x + (entity->hitbox.width - 1), .y = entity->hitbox.y};
    entity->hitbox_l = (Rectangle){.width = 1, .height = entity->hitbox.height - 1, .x = entity->hitbox.x, .y = entity->hitbox.y};
    entity->hitbox_u = (Rectangle){.width = entity->hitbox.width - 2, .height = 1, .x = entity->hitbox.x + 1, .y = entity->hitbox.y};
    entity->hitbox_d = (Rectangle){.width = entity->hitbox.width - 2, .height = 1, .x = entity->hitbox.x + 1, .y = entity->hitbox.y + (entity->hitbox.height - 1)};
}