#include <fightable/mixer.h>
#include <fightable/sanitizer.h>

float _fCalculateValueMix(float a, float b, float equality) {
	return a + ((b - a) * _fSanitizeFloat(equality, 0.f, 1.f));
}