#include <fightable/sound_engine.h>
#include <fightable/mixer.h>
#include <fightable/sanitizer.h>
#include <raylib.h>

void _fAudioFxUpdate(struct faudio_engine* engine) {
	float delta = GetFrameTime();
	
	if (engine->fx.perform_volume_slider) {
		const float ratio = 1 / engine->fx.slide_volume_time;
		const float mix = _fCalculateValueMix(engine->fx.slide_volume_old, engine->fx.slide_volume_value, engine->fx.slide_volume_cur_time * ratio);

		_fAudioSetVolume(engine, mix);
		engine->fx.slide_volume_cur_time += delta;

		if (engine->fx.slide_volume_cur_time > engine->fx.slide_volume_time) {
			engine->fx.perform_volume_slider = 0;
			_fAudioSetVolume(engine, engine->fx.slide_volume_value);
		}
	}
}
void _fAudioFxSlideVolume(struct faudio_engine* engine, float v, float time) {
	if (time <= 0) return _fAudioSetVolume(engine, v);

	engine->fx.perform_volume_slider = 1;
	engine->fx.slide_volume_old = _fAudioGetVolume(engine);
	engine->fx.slide_volume_value = _fSanitizeFloat(v, 0, 2);
	engine->fx.slide_volume_time = time;
	engine->fx.slide_volume_cur_time = 0;
}