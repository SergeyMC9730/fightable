#include <fightable/sound_engine.h>
#include <fightable/sanitizer.h>

#define VOLUME_MIN 0
#define VOLUME_MAX 2

float _fAudioGetVolume(struct faudio_engine *engine) {
    return _fSanitizeFloat(engine->volume, VOLUME_MIN, VOLUME_MAX);
}
void _fAudioSetVolume(struct faudio_engine *engine, float v) {
    engine->volume = _fSanitizeFloat(v, VOLUME_MIN, VOLUME_MAX);
}