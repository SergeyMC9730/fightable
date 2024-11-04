#pragma once

#define ENTITY_PLAYER 1

#include <raylib.h>
#include <stdint.h>

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
#include <fightable/serializable.h>

struct fentity {
    Vector2 speed;

    unsigned char on_ground : 1;
    unsigned char moving_horizontally : 1;
    unsigned char jumping : 1;
    unsigned char moving_negative : 1;
    unsigned char complete_px : 1;
    unsigned char complete_nx : 1;
    unsigned char can_double_jump : 1;

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

#define ENTITY_SIZE (                       \   
    sizeof(float) + /* hitbox.x */          \ 
    sizeof(float) + /* hitbox.y */          \
    sizeof(short) + /* global_entity_id */  \
    sizeof(short)   /* entity_id */         \
)

#ifdef WITH_PLACEHOLDERS
void _fEntityUpdate(struct fentity *entity);
void _fEntityDraw(struct fentity *entity);
#endif

void _fEntityMove(struct fentity* entity, Vector2 pos);
void _fEntityInit(struct fentity* entity);
void _fEntityJump(struct fentity* entity, unsigned char holding);

#include <fightable/intvec.h>

IVector2 _fEntityGetDrawingPos(struct fentity* entity);

void _fEntitySetHitbox(struct fentity* entity, Rectangle rec);
void _fEntitySetPosition(struct fentity* entity, Vector2 pos);
fserializable _fEntitySerialize(struct fentity* entity);
struct fentity* _fEntityLoad(fserializable *serializable, uint16_t level_version);

#ifdef __cplusplus
}
#endif