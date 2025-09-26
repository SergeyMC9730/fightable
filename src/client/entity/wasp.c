
//          Sergei Baigerov 2024 - 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include "fightable/distance.h"
#include "fightable/hitbox.h"
#include "fightable/renderer.h"
#include "fightable/sound_engine.h"
#include "fightable/storage.h"
#include "nt5emul/renderer_event.h"
#include "rsb/rsb_array_gen.h"
#define WITH_PLACEHOLDERS
#include "fightable/entity.h"
#include "fightable/intvec.h"
#include "fightable/tilemap.h"
#include <fightable/e_wasp.h>
#include <fightable/state.h>
#include <fightable/string.h>
#include <fightable/color.h>
#include <fightable/entity_library.h>
#include <fightable/level.h>

RSB_ARRAY_IMPL_GEN(struct fewasp_state_entry, _wasp_se);

void _feWaspInit(struct fentity_wasp* instance) {
	if (!instance) return;

	_fEntityInit(&instance->base);

	instance->base.hitbox.width = 0;
	instance->base.hitbox.height = 0;

	instance->base.can_be_damaged = 0;
	instance->base.no_gravity = 1;
	instance->base.global_entity_id = ENTITY_WASP;

	instance->base.draw = (void(*)(struct fentity*))_feWaspDraw;
	instance->base.cleanup = (void(*)(struct fentity*))_feWaspCleanup;
	instance->base.update = (void(*)(struct fentity*))_feWaspUpdate;

	IVector2 t = __state.tilemap2->tile_size;
	instance->temp_img = GenImageColor(t.x, t.y, BLANK);
	instance->temp_img2 = GenImageColor(t.x, t.y, BLANK);
	instance->main_texture = LoadTextureFromImage(instance->temp_img);
	instance->texture2 = _fTilemapExportTile(__state.tilemap2, (IVector2){4, 0});

	instance->tile1 = _fTilemapExportTileAsImage(__state.tilemap2, (IVector2){0, 0});
	instance->tile2 = _fTilemapExportTileAsImage(__state.tilemap2, (IVector2){3, 0});

	instance->trapped_entities = RSBCreateArray_wasp_se();

	// SetTextureFilter(instance->main_texture, TEXTURE_FILTER_BILINEAR);
	SetTextureFilter(instance->texture2, TEXTURE_FILTER_BILINEAR);
}
void _feWaspDraw(struct fentity_wasp* instance) {
	if (!instance) return;

	RLRectangle rect = {0, 0, instance->temp_img.width, instance->temp_img.height};

	ImageClearBackground(&instance->temp_img, BLANK);
	ImageDraw(&instance->temp_img, instance->tile1, rect, rect, WHITE);

	int offset = ((int)(instance->live_time * (float)instance->tile2.width)) % instance->tile2.width;

	RLRectangle rect1s, rect1d;

	ImageClearBackground(&instance->temp_img2, BLANK);

	int x = offset;
	int y = offset;

	{
    	rect1s = (RLRectangle){x, y, instance->tile2.width - x, instance->tile2.height - y};
    	rect1d = (RLRectangle){0, 0, rect1s.width, rect1s.height};
    	ImageDraw(&instance->temp_img2, instance->tile2, rect1s, rect1d, WHITE);
     	rect1s = (RLRectangle){0, y, x, instance->tile2.height - y};
        rect1d = (RLRectangle){instance->tile2.width - x, 0, rect1s.width, rect1s.height};
        ImageDraw(&instance->temp_img2, instance->tile2, rect1s, rect1d, WHITE);
        rect1s = (RLRectangle){x, 0, instance->tile2.width - x, y};
        rect1d = (RLRectangle){0, instance->tile2.height - y, rect1s.width, rect1s.height};
        ImageDraw(&instance->temp_img2, instance->tile2, rect1s, rect1d, WHITE);
        rect1s = (RLRectangle){0, 0, x, y};
        rect1d = (RLRectangle){instance->tile2.width - y, instance->tile2.height - y, rect1s.width, rect1s.height};
        ImageDraw(&instance->temp_img2, instance->tile2, rect1s, rect1d, WHITE);
	}

	for (int x = 0; x < instance->temp_img.width; x++) {
        for (int y = 0; y < instance->temp_img.height; y++) {
            Color c1 = GetImageColor(instance->temp_img, x, y);

            if (c1.a != 0) {
                Color c2 = GetImageColor(instance->temp_img2, x, y);

                FColor fcol1 = _fColorToFloat(c1);
                FColor fcol2 = _fColorToFloat(c2);

                fcol1.r += (fcol2.r * fcol2.a);
                fcol1.g += (fcol2.g * fcol2.a);
                fcol1.b += (fcol2.b * fcol2.a);

                ImageDrawPixel(&instance->temp_img, x, y, _fFloatToColor(fcol1));
            }
        }
    }

    // ImageDraw(&instance->temp_img, instance->temp_img2, rect, rect, WHITE);

	UpdateTexture(instance->main_texture, instance->temp_img.data);

	IVector2 pos = _fEntityGetDrawingPos(&instance->base);
	IVector2 t = __state.tilemap2->tile_size;

	RLRectangle source = {0, 0, instance->main_texture.width, instance->main_texture.height};
	RLRectangle dest = {pos.x + source.width / 2, pos.y + source.height / 2, instance->main_texture.width, instance->main_texture.height};

	double l = instance->live_time;
	int degrees = (int)(fabs(sin(l) * cos(2 * l) + (l / 10)) * 360) % 360;

	BeginBlendMode(BLEND_ADD_COLORS);
	Color col = (Color){0, 32, 64, 32};
	DrawCircleGradient(pos.x + (t.x / 2), pos.y + (t.y / 2), (float)t.x * 1.f * ((sin(instance->live_time * 50) + 7) / 8), col, BLANK);
	EndBlendMode();
	DrawTexturePro(instance->main_texture, source, dest, (Vector2){source.width / 2, source.height / 2}, degrees, SKYBLUE);
	_fTilemapDraw(__state.tilemap2, pos, (IVector2){1, 0}, 0, 0, WHITE);
	// _fTilemapDraw(__state.tilemap2, pos, (IVector2){2, 0}, 0, 0, WHITE);
	degrees /= -2;
	BeginBlendMode(BLEND_MULTIPLIED);
	col = (Color){128, 128, 128, 32};
	DrawTexturePro(instance->texture2, source, dest, (Vector2){source.width / 2, source.height / 2}, degrees, col);
	EndBlendMode();
	BeginBlendMode(BLEND_ADDITIVE);
	DrawCircleGradient(pos.x + (t.x / 2), pos.y + (t.y / 2), (float)t.x * 1.5f * ((sin(instance->live_time * 25) + 7) / 8), col, BLANK);
	EndBlendMode();

	instance->live_frames++;
	instance->live_time += GetFrameTime();
	instance->live_time = fmod(instance->live_time, 2.4 * PI);

	IVector2 center = {
	    pos.x + (instance->main_texture.width / 2),
		pos.y + (instance->main_texture.height / 2)
	};

	IVector2 tt = instance->base.level->tilemap->tile_size;

	//DrawCircleV(_fImathToVFloat(center), 32 * instance->base.level->tilemap->texture.width, RED);
	DrawCircleLines(center.x, center.y, 32 * tt.x, RED);
	DrawCircleLines(center.x, center.y, 8 * tt.x, YELLOW);
	DrawCircleLines(center.x, center.y, 48 * tt.x, BLUE);

	// TraceLog(LOG_INFO, "%lf %d", instance->live_time, degrees * -2);
}
void _feWaspCleanup(struct fentity_wasp* instance) {
    TraceLog(LOG_INFO, "BBBBBB");

	if (!instance) return;

	TraceLog(LOG_INFO, "AAAAAA");
	TraceLog(LOG_INFO, "AAAAAA");
	TraceLog(LOG_INFO, "AAAAAA");
	TraceLog(LOG_INFO, "AAAAAA");
	TraceLog(LOG_INFO, "AAAAAA");
	TraceLog(LOG_INFO, "AAAAAA");
	TraceLog(LOG_INFO, "AAAAAA");
	TraceLog(LOG_INFO, "AAAAAA");
	TraceLog(LOG_INFO, "AAAAAA");

	UnloadImage(instance->tile1);
	UnloadImage(instance->tile2);
	UnloadImage(instance->temp_img);
	UnloadImage(instance->temp_img2);
	UnloadTexture(instance->main_texture);
	UnloadTexture(instance->texture2);

    for (unsigned int i = 0; i < instance->trapped_entities->len; i++) {
        struct fewasp_state_entry *entry = instance->trapped_entities->objects + i;

        TraceLog(LOG_INFO, "SADAKLSJDSAD");

        _fAudioSetSoundParamRl(&__state.sound_engine, entry->sounds.drone_near_loop, AE_LOOP, 0);
        _fAudioRunSoundActionRl(&__state.sound_engine, entry->sounds.drone_near_loop, AE_STOP);

        _fAudioSetSoundParamRl(&__state.sound_engine, entry->sounds.drone_close, AE_LOOP, 0);
        _fAudioRunSoundActionRl(&__state.sound_engine, entry->sounds.drone_close, AE_STOP);

        _fAudioSetSoundParamRl(&__state.sound_engine, entry->sounds.drone_flew, AE_LOOP, 0);
        _fAudioRunSoundActionRl(&__state.sound_engine, entry->sounds.drone_flew, AE_STOP);
    }

	RSBDestroy_wasp_se(instance->trapped_entities);
}

struct fewasp_state_entry *_feWaspGetTrappedEntityState(struct fentity_wasp* instance, struct fentity *e) {
    if (!instance || !e || &instance->base == e || !instance->trapped_entities) return NULL;

    for (unsigned int i = 0; i < instance->trapped_entities->len; i++) {
        struct fewasp_state_entry *entry = instance->trapped_entities->objects + i;
        if (entry->locked_entity == e) return entry;
    }

    return NULL;
}

double _feWaspDistanceFromEntity(struct fentity_wasp* instance, struct fentity *e) {
    IVector2 t = instance->base.level->tilemap->tile_size;
    IVector2 pos = (IVector2){instance->base.hitbox.x, instance->base.hitbox.y};
    IVector2 pos2 = (IVector2){e->hitbox.x, e->hitbox.y};
    IVector2 center = {
        pos.x + (instance->main_texture.width / 2),
        pos.y + (instance->main_texture.height / 2)
    };

    return _fDistPointToPoint(_fImathToVFloat(center), _fImathToVFloat(pos2));
}

void _feWaspDamagePlayer(void *_ctx) {
    struct fentity *entity = (struct fentity *)_ctx;

    return; // TEMP

    entity->damage(entity, 5.f);
}

unsigned char _feWaspEntityInRadius(struct fentity_wasp* instance, struct fentity *e, unsigned int radius_blocks) {
    IVector2 t = instance->base.level->tilemap->tile_size;
    IVector2 pos = (IVector2){instance->base.hitbox.x, instance->base.hitbox.y};
    IVector2 center = {
        pos.x + (instance->main_texture.width / 2),
        pos.y + (instance->main_texture.height / 2)
    };

    return CheckCollisionCircleRec(_fImathToVFloat(center), radius_blocks * t.x, _fHitboxToRect(e->hitbox));
}

void _feWaspUpdate(struct fentity_wasp* instance) {
    if (!instance) return;

    _fEntityUpdate(&instance->base);

    struct flevel *level = instance->base.level;
    if (!level) return;

    IVector2 pos = (IVector2){instance->base.hitbox.x, instance->base.hitbox.y};
    IVector2 center = {
        pos.x + (instance->main_texture.width / 2),
        pos.y + (instance->main_texture.height / 2)
    };

    IVector2 t = instance->base.level->tilemap->tile_size;

    double closest_distance;
    struct fentity *closest_target;

    for (unsigned int i = 0; i < level->entities->len; i++) {
        struct fentity *entity = level->entities->objects[i];
        if (!entity) continue;

        if (entity->global_entity_id == ENTITY_WASP) continue;
        if (entity->global_entity_id == ENTITY_PLAYER) {
            float distance_circ = _fDistPointToCircle((Vector2){entity->hitbox.x, entity->hitbox.y}, _fImathToVFloat(center), 32 * t.y);
            float distance = _feWaspDistanceFromEntity(instance, entity);

            if (_feWaspEntityInRadius(instance, entity, 8)) {
                struct fewasp_state_entry *current = _feWaspGetTrappedEntityState(instance, entity);
                if (!current) {
                    struct fewasp_state_entry new_entry = {};
                    new_entry.locked_entity = entity;
                    RSBAddElement_wasp_se(instance->trapped_entities, new_entry);
                    current = _feWaspGetTrappedEntityState(instance, entity);
                }

                if (!current->played_effect_1) {
                    current->sounds.drone_close = _fAudioPlayRaylibSound(&__state.sound_engine, "assets/sounds/drone_close.wav", 1);
                    _fAudioSetSoundParamRl(&__state.sound_engine, current->sounds.drone_close, AE_VOLUME, 1.f);
                    current->played_effect_1 = 1;
                }

                _fScheduleOverlayFunc((renderer_event_t){_feWaspDamagePlayer, entity});
            }
            if (!_feWaspEntityInRadius(instance, entity, 32)) {
                struct fewasp_state_entry *current = _feWaspGetTrappedEntityState(instance, entity);
                if (current) {
                    if (!current->played_effect_2) {
                        current->sounds.drone_flew = _fAudioPlayRaylibSound(&__state.sound_engine, "assets/sounds/drone_flew.wav", 1);
                        _fAudioSetSoundParamRl(&__state.sound_engine, current->sounds.drone_flew, AE_VOLUME, 0.5f);
                        current->played_effect_2 = 1;
                        current->played_effect_1 = 0;
                        current->played_effect_3 = 0;

                        _fAudioSetSoundParamRl(&__state.sound_engine, current->sounds.drone_near_loop, AE_LOOP, 0);
                        _fAudioRunSoundActionRl(&__state.sound_engine, current->sounds.drone_near_loop, AE_STOP);
                    }
                }
            } else {
                struct fewasp_state_entry *current = _feWaspGetTrappedEntityState(instance, entity);
                if (!current) {
                    struct fewasp_state_entry new_entry = {};
                    new_entry.locked_entity = entity;
                    RSBAddElement_wasp_se(instance->trapped_entities, new_entry);
                    current = _feWaspGetTrappedEntityState(instance, entity);
                }

                if (!current->played_effect_3) {
                    char *buffer = (char *)MemAlloc(512);
                    const char *storage = _fStorageGetWritable();

                    current->sounds.drone_near_loop = _fAudioPlayRaylibSound(&__state.sound_engine, "assets/sounds/drone_near_loop.wav", 1);
                    _fAudioSetSoundParamRl(&__state.sound_engine, current->sounds.drone_near_loop, AE_LOOP, 1);
                    _fAudioSetSoundParamRl(&__state.sound_engine, current->sounds.drone_near_loop, AE_VOLUME, 0.5f);

                    current->played_effect_3 = 1;
                    current->played_effect_2 = 0;
                } else {
                    float cd = 1.f;
                    if (entity->max_damage_colddown > 0.f) {
                        cd = 1.f - (entity->damage_colddown / entity->max_damage_colddown);
                    }
                    float nv = 0.5f * distance_circ * cd;
                    TraceLog(LOG_INFO, "nv=%f distance_circ=%f cd=%f distance=%f", nv, distance_circ, cd, distance);

                    _fAudioSetSoundParamRl(&__state.sound_engine, current->sounds.drone_near_loop, AE_VOLUME, nv);
                }
            }
        }
    }
}
