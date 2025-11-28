#pragma once

#ifdef __cplusplus
extern "C" {
#endif

struct fightable_init_data {
    void *render_surface;
    unsigned char (*draw)();
    unsigned char (*close)();
};

void _fInit(int argc, char **argv, struct fightable_init_data *data);

#ifdef __cplusplus
}
#endif
