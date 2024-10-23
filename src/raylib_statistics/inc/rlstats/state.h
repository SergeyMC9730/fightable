#pragma once

struct raylib_statistics_state {
    unsigned int draw_calls;
};

extern struct raylib_statistics_state __state;