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

#include <fightable/hitbox.h>

struct fentity {
    Vector2 speed;

    unsigned char on_ground : 1;
    unsigned char moving_horizontally : 1;
    unsigned char jumping : 1;
    unsigned char moving_negative : 1;
    unsigned char complete_px : 1;
    unsigned char complete_nx : 1;

    fhitbox hitbox;

    fhitbox* obstacles;
    unsigned int obstacles_length;

    fhitbox ground_hitbox;

    struct flevel *level;

    unsigned char render_direction;

    unsigned short global_entity_id;
    unsigned short entity_id;

    fhitbox standing_object;

    void (*update)(struct fentity *entity);
    void (*draw)(struct fentity *entity);
};

#pragma pack(pop)

#ifdef WITH_PLACEHOLDERS
void _fEntityUpdate(struct fentity *entity);
void _fEntityDraw(struct fentity *entity);
#endif

void _fEntityMove(struct fentity* entity, Vector2 pos);
void _fEntityInit(struct fentity* entity);
void _fEntityJump(struct fentity* entity);

#ifdef __cplusplus
}
#endif