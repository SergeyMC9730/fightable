#include <fightable/color.h>

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