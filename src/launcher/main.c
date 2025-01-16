#include <fightable/init.h>
#include <stdio.h>

int main(int argc, char **argv) {
    static const char *creators[] = {
        "SergeyMC9730", "InviseDivine", "Kolyah35"
    };

    static const char *fightable_renderer = "raylib";

    printf("! Fightable\n");
    printf("! Made by ");

    unsigned int crlen = sizeof (creators) / sizeof(const char *);

    for (unsigned int i = 0; i < crlen; i++) {
        printf("%s", creators[i]);
        if ((i + 1) != crlen) {
            printf(", ");
        }
    }
    printf("\n");

    printf("! Powered by %s\n", fightable_renderer);

    _fInit(argc, argv);

    return 0;
}
