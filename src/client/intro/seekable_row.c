#include <fightable/intro.h>
#include <fightable/state.h>

int _fIntroGetSeekableRow() {
    switch (__state.song_id) {
        case 0: {
            return 5;
        }
        case 1: {
            return 3;
        }
        default: {
            return -1;
        }
    }
}