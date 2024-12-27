#include <fightable/color.h>
#include <fightable/sanitizer.h>
#include <fightable/mixer.h>

FColor _fColorToFloat(Color col) {
    FColor res = {
        .r = (float)col.r / 255.f,
        .g = (float)col.g / 255.f,
        .b = (float)col.b / 255.f,
        .a = (float)col.a / 255.f
    };

    return res;
}
Color _fFloatToColor(FColor col) {
    Color res = {
        .r = (unsigned char)(col.r * 255.f),
        .g = (unsigned char)(col.g * 255.f),
        .b = (unsigned char)(col.b * 255.f),
        .a = (unsigned char)(col.a * 255.f),
    };

    return res;
}

Color _fInvertColor(Color col) {
    Color res = col;
    
    res.r = 255 - col.r;
    res.g = 255 - col.g;
    res.b = 255 - col.b;
    
    return res;
}
FColor _fInvertFColor(FColor col) {
    FColor res = col;
    
    res.r = 1.f - col.r;
    res.g = 1.f - col.g;
    res.b = 1.f - col.b;

    return res;
}

void _fFColorSanitize(FColor *col) {
    col->a = _fSanitizeFloat(col->a, 0.f, 1.f);
    col->r = _fSanitizeFloat(col->r, 0.f, 1.f);
    col->g = _fSanitizeFloat(col->g, 0.f, 1.f);
    col->b = _fSanitizeFloat(col->b, 0.f, 1.f);
}

Color _fMixColors(Color a, Color b, float equality) {
    // prevent nan and equality overflow situations
    if (equality >= 1.f) return b;
    if (equality <= 0.f) return a;

    Color newc = {};

    newc.r = (unsigned char)_fCalculateValueMix((float)a.r, (float)b.r, equality);
    newc.g = (unsigned char)_fCalculateValueMix((float)a.g, (float)b.g, equality);
    newc.b = (unsigned char)_fCalculateValueMix((float)a.b, (float)b.b, equality);
    newc.a = (unsigned char)_fCalculateValueMix((float)a.a, (float)b.a, equality);

    return newc;
}