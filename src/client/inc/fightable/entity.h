#pragma once

#define ENTITY_PLAYER 1

#include <raylib.h>

#ifdef __cplusplus
extern "C" {
#endif

struct flevel;

#pragma pack(push, 1)

enum fentity_direction {
    ENTITY_DIR_RIGHT = 0,
    ENTITY_DIR_LEFT = 1
};

struct fentity {
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

    Rectangle hitbox;
    Rectangle *obstacles;
    unsigned int obstacles_length;

    Rectangle hitbox_r;
    Rectangle hitbox_l;
    Rectangle hitbox_u;
    Rectangle hitbox_d;

    Rectangle standing_surface;
    Rectangle ceiling_surface;
    Rectangle collided_surface;

    unsigned char ccheck0 : 1;
    unsigned char ccheck1 : 1;
    unsigned char debug_draw : 1;
    unsigned char no_ai : 1;

    unsigned int frames_since_jump;

    struct flevel *level;

    enum fentity_direction render_direction;

    double delta;

    unsigned short global_entity_id;
    unsigned short entity_id;

    void (*update)(struct fentity *entity);
    void (*draw)(struct fentity *entity);
};

#pragma pack(pop)

#ifdef WITH_PLACEHOLDERS
void _fEntityUpdate(struct fentity *entity);
void _fEntityDraw(struct fentity *entity);
#endif

#ifdef __cplusplus
}
#endif