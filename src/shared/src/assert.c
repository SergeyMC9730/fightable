#include <stdio.h>
#include <wchar.h>
#include <locale.h>
#include <stdlib.h>

void _wassert(wchar_t const* message, wchar_t const* filename, unsigned line) {
    setlocale(LC_ALL, "");
    printf("%ls:%d: Assertion `%ls` failed.", filename, line, message);

    abort();
}