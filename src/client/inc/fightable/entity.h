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

struct fentity_accessory {
    unsigned char type;
};

#include <rsb/rsb_array_gen.h>

RSB_ARRAY_DEF_GEN(struct fentity_accessory, _fentity_accessory);

enum fentity_accessory_obj {
    ENTITY_ACC_NONE         = 0,
    ENTITY_ACC_HAT_1        = 1,
    ENTITY_ACC_HAT_2        = 2,
    ENTITY_ACC_HAT_3        = 3,
    ENTITY_ACC_GLASSES_1    = 4
};
enum fentity_accessory_type {
    ENTITY_ACCT_NONE = 0,
    ENTITY_ACCT_HAT = 1,
    ENTITY_ACCT_GLASSES = 2
};

struct fentity {
    Vector2 speed;

    unsigned char on_ground : 1;
    unsigned char moving_horizontally : 1;
    unsigned char jumping : 1;
    unsigned char moving_negative : 1;
    unsigned char complete_px : 1;
    unsigned char complete_nx : 1;
    unsigned char can_double_jump : 1;
    unsigned char can_be_damaged : 1;

    unsigned char dead : 1;
    unsigned char object_destroyed : 1;
    unsigned char begin_destruction : 1;

    fhitbox hitbox;

    fhitbox* obstacles;
    unsigned int obstacles_length;

    fhitbox ground_hitbox;

    struct flevel *level;

    unsigned char render_direction;

    unsigned short global_entity_id;
    unsigned short entity_id;

    fhitbox standing_object;

    rsb_array__fentity_accessory* accessories;

    float max_damage_colddown;
    float damage_colddown;

    float max_hp;
    float hp;

    Color tint;

    float destroy_timer;

    float custom_delta;

    void (*update)(struct fentity *entity);
    void (*draw)(struct fentity *entity);
    void (*cleanup)(struct fentity* entity);
    void (*damage)(struct fentity* entity, float damage);
    void (*kill)(struct fentity* entity);
    void (*perform_death)(struct fentity* entity);
};

#pragma pack(pop)

// hitbox.x + hitbox.y + global_entity_id + entity_id
#define ENTITY_SIZE (sizeof(float) + sizeof(float) + sizeof(short) + sizeof(short))

#ifdef WITH_PLACEHOLDERS
void _fEntityUpdate(struct fentity *entity);
void _fEntityDraw(struct fentity *entity);
void _fEntityCleanup(struct fentity* entity);
void _fEntityDamage(struct fentity* entity, float damage);
void _fEntityKill(struct fentity* entity);
#endif

void _fEntityMove(struct fentity* entity, Vector2 pos);
void _fEntityInit(struct fentity* entity);
void _fEntityJump(struct fentity* entity, unsigned char holding);

#include <fightable/intvec.h>

IVector2 _fEntityGetDrawingPos(struct fentity* entity);

void _fEntitySetHitbox(struct fentity* entity, RLRectangle rec);
void _fEntitySetPosition(struct fentity* entity, Vector2 pos);
fserializable _fEntitySerialize(struct fentity* entity);
struct fentity* _fEntityLoad(fserializable *serializable, uint16_t level_version);

void _fEntityDrawAccessory(struct fentity* entity);
void _fEntityAddAccessory(struct fentity* entity, enum fentity_accessory_obj accessory);

enum fentity_accessory_type _fAccessoryGetType(enum fentity_accessory_obj accessory);

void _fEntitySetDestroyTimer(struct fentity* entity, float time);

void _fEntitySetDelta(struct fentity* entity, float delta);
float _fEntityGetDelta(struct fentity* entity);

#ifdef __cplusplus
}
#endif
