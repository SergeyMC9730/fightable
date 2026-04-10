
//          Sergei Baigerov 2024 - 2026.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#define WITH_PLACEHOLDERS
#include <fightable/etext.h>
#include <fightable/text.h>
#include <fightable/state.h>
#include <fightable/string.h>
#include <fightable/entity_library.h>

void _feTextInit(struct fentity_text* instance, const char* text) {
	if (text == NULL) return _feTextInit(instance, "<cred>?");
	if (instance == NULL) return;

	_fEntityInit(&instance->base);

	instance->text = _fMultilineTextInstanceCreateWithTextMan(text, &__state.text_manager);

	Vector2 v = _fMultilineTextInstanceGetSize(instance->text);
	fhitbox hitbox = instance->base.hitbox.hitbox;
	hitbox.width = v.x; hitbox.height = v.y;

	instance->base.can_be_damaged = 0;
	_fEntitySetHitbox(&instance->base, hitbox);
	instance->base.global_entity_id = ENTITY_TEXT;
	instance->base.draw = (void(*)(struct fentity*))_feTextDraw;
	instance->base.cleanup = (void(*)(struct fentity*))_feTextCleanup;
}
void _feTextDraw(struct fentity_text* instance) {
	if (!instance || !instance->text) return;

	IVector2 pos = _fEntityGetDrawingPos(&instance->base);
	// _fTextDraw(&__state.text_manager, (const char*)instance->text, pos, instance->base.tint, 0);
	_fMultilineTextInstanceDraw(instance->text, _fImathToVFloat(pos));
}
void _feTextCleanup(struct fentity_text* instance) {
	if (!instance || !instance->text) return;

	_fMultilineTextInstanceDestroy(instance->text);
	instance->text = NULL;
}
