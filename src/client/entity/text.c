#define WITH_PLACEHOLDERS
#include <fightable/etext.h>
#include <fightable/text.h>
#include <fightable/state.h>
#include <fightable/string.h>
#include <fightable/entity_library.h>

void _feTextInit(struct fentity_text* instance, const char* text) {
	if (text == NULL) return _feTextInit(instance, "?");
	if (instance == NULL) return;

	_fEntityInit(&instance->base);

	IVector2 v = _fTextMeasure(&__state.text_manager, text);
	fhitbox hitbox = instance->base.hitbox;
	hitbox.width = (float)v.x; hitbox.height = (float)v.y;
	
	instance->base.can_be_damaged = 0;
	instance->base.hitbox = hitbox;
	instance->text = _fCopyString(text);
	instance->base.global_entity_id = ENTITY_TEXT;
	instance->base.draw = (void(*)(struct fentity*))_feTextDraw;
	instance->base.cleanup = (void(*)(struct fentity*))_feTextCleanup;
}
void _feTextDraw(struct fentity_text* instance) {
	if (!instance || !instance->text) return;

	IVector2 pos = _fEntityGetDrawingPos(&instance->base);
	_fTextDraw(&__state.text_manager, (const char*)instance->text, pos, instance->base.tint, 0);
}
void _feTextCleanup(struct fentity_text* instance) {
	if (!instance || !instance->text) return;

	free(instance->text);
	instance->text = NULL;
}