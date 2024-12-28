#define WITH_PLACEHOLDERS

#include <fightable/entity.h>

void _fEntityDamage(struct fentity* entity, float damage) {
	if (!entity || entity->dead || entity->damage_colddown > 0 || entity->object_destroyed) return;

	entity->hp -= damage;
	if (entity->hp < 0) {
		entity->dead = 1;
		entity->hp = 0;

		if (entity->perform_death) entity->perform_death(entity);
	}

	entity->damage_colddown = entity->max_damage_colddown;
}

void _fEntityKill(struct fentity* entity) {
	if (!entity || entity->object_destroyed) return;

	if (!entity->damage) {
		_fEntityDamage(entity, entity->max_hp);
	}
	else {
		entity->damage(entity, entity->max_hp);
	}
}