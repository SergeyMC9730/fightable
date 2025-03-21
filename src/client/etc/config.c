
//          Sergei Baigerov 2024 - 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include <fightable/config.h>
#include <fightable/storage.h>
#include <fightable/state.h>
#include <cJSON.h>
#include <stdio.h>

void _fConfigSave(struct fconfig* config) {
	if (!config) return;

	cJSON* main = cJSON_CreateObject();
	cJSON* vsync = cJSON_CreateBool(config->vsync_flag);
	cJSON* fullscreen = cJSON_CreateBool(config->fullscreen_flag);
	cJSON* volume = cJSON_CreateNumber(config->volume_slider.progress);

	cJSON_AddItemReferenceToObject(main, "vsync", vsync);
	cJSON_AddItemReferenceToObject(main, "fullscreen", fullscreen);
	cJSON_AddItemReferenceToObject(main, "volume", volume);

	char* str = cJSON_Print(main);
	char* buffer = (char*)MemAlloc(2048);

	snprintf(buffer, 2048, "%s/config.json", _fStorageGetWritable());
	SaveFileText(buffer, str);

	MemFree(buffer);
	cJSON_free(str);
	cJSON_Delete(main);
}
void _fConfigRead(struct fconfig* config) {
	if (!config) return;

	char* buffer = (char*)MemAlloc(2048);

	snprintf(buffer, 2048, "%s/config.json", _fStorageGetWritable());

	if (!FileExists(buffer)) {
		MemFree(buffer);
		return;
	}

	char* str = LoadFileText(buffer);
	if (!str) {
		MemFree(buffer);
		return;
	}

	cJSON* main = cJSON_Parse(str);
	cJSON* vsync = cJSON_GetObjectItem(main, "vsync");
	cJSON* fullscreen = cJSON_GetObjectItem(main, "fullscreen");
	cJSON* volume = cJSON_GetObjectItem(main, "volume");

	if (vsync && cJSON_IsBool(vsync)) {
		config->vsync_flag = vsync->valueint;
	}
	if (fullscreen && cJSON_IsBool(fullscreen)) {
		config->fullscreen_flag = volume->valueint;
	}
	if (volume && cJSON_IsNumber(volume)) {
		config->volume_slider.progress = volume->valuedouble;
	}

	MemFree(buffer);
	MemFree(str);
	cJSON_Delete(main);

	__state.sound_engine.volume = config->volume_slider.progress;
}

void _fConfigInit(struct fconfig* config) {
	if (!config) return;

	_fConfigRead(config);

	struct fsquare_button btn = { 0 };
	IVector2 btncheck_pos = { 10, 52 };
	int w = 80;
	RLRectangle area = (RLRectangle){ btncheck_pos.x + 10 + 2, btncheck_pos.y + 1, w, 10 };

	btn.position = (IVector2){ 10, area.y + 5 };
	btn.tint = WHITE;
	btn.flag = config->vsync_flag;
	btn.type = SQRBTN_REVCHECKBOX;

	config->vsync_btn = btn;

	struct fslider slider = { .progress = config->volume_slider.progress };
	slider.tint = BLUE;
	slider.scaling = 1.f;
	slider.rect = (RLRectangle){ 10, 10, 30, 4 };
	slider.movable_width = 4.f;

	config->volume_slider = slider;

	struct fsquare_button btn_fullscreen = { 0 };
	btn_fullscreen.tint = WHITE;
	btn_fullscreen.position = (IVector2){ 10, area.y + 20 };
	btn_fullscreen.type = SQRBTN_CHECKBOX;
	btn_fullscreen.flag = config->fullscreen_flag;

	config->fullscreen_btn = btn_fullscreen;
}
