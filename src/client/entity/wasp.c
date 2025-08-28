#include "fightable/intvec.h"
#include "fightable/tilemap.h"
#include "raylib.h"
#define WITH_PLACEHOLDERS
#include <fightable/e_wasp.h>
#include <fightable/state.h>
#include <fightable/string.h>
#include <fightable/color.h>
#include <fightable/entity_library.h>

void _feWaspInit(struct fentity_wasp* instance) {
	if (instance == NULL) return;

	_fEntityInit(&instance->base);

	instance->base.hitbox.width = 0;
	instance->base.hitbox.height = 0;

	instance->base.can_be_damaged = 0;
	instance->base.no_gravity = 1;
	instance->base.global_entity_id = ENTITY_WASP;

	instance->base.draw = (void(*)(struct fentity*))_feWaspDraw;
	instance->base.cleanup = (void(*)(struct fentity*))_feWaspCleanup;

	IVector2 t = __state.tilemap2->tile_size;
	instance->temp_img = GenImageColor(t.x, t.y, BLANK);
	instance->temp_img2 = GenImageColor(t.x, t.y, BLANK);
	instance->main_texture = LoadTextureFromImage(instance->temp_img);
	instance->texture2 = _fTilemapExportTile(__state.tilemap2, (IVector2){4, 0});

	instance->tile1 = _fTilemapExportTileAsImage(__state.tilemap2, (IVector2){0, 0});
	instance->tile2 = _fTilemapExportTileAsImage(__state.tilemap2, (IVector2){3, 0});

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

	UpdateTexture(instance->main_texture, instance->temp_img.data);

	IVector2 pos = _fEntityGetDrawingPos(&instance->base);
	IVector2 t = __state.tilemap2->tile_size;

	RLRectangle source = {0, 0, instance->main_texture.width, instance->main_texture.height};
	RLRectangle dest = {pos.x + source.width / 2, pos.y + source.height / 2, instance->main_texture.width, instance->main_texture.height};

	double degrees = fmod(instance->live_time * 30, 360) * (cos(instance->live_time) + sin(instance->live_time + 3.14f));

	BeginBlendMode(BLEND_ADD_COLORS);
	Color col = (Color){0, 32, 64, 32};
	DrawCircleGradient(pos.x + (t.x / 2), pos.y + (t.y / 2), (float)t.x * 1.f * ((sin(instance->live_time * 25) + 7) / 8), col, BLANK);
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
	instance->live_time = fmod(instance->live_time, 6.f);
}
void _feWaspCleanup(struct fentity_wasp* instance) {
	if (!instance) return;

	UnloadImage(instance->tile1);
	UnloadImage(instance->tile2);
	UnloadImage(instance->temp_img);
	UnloadImage(instance->temp_img2);
	UnloadTexture(instance->main_texture);
	UnloadTexture(instance->texture2);
}
