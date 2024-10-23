#include <stdio.h>
#include <unistd.h>
#include <dlfcn.h>
#include <string.h>

int fputs(const char *message, FILE *file) {
    int (*new_fputs)(const char *message, FILE *file) = dlsym(RTLD_NEXT, "fputs");

    return new_fputs("lol 2", file);
}

int puts(const char *message) {
    int (*new_puts)(const char *message) = dlsym(RTLD_NEXT, "puts");

    return new_puts("lol");
}