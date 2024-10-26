#pragma once

#include <map>

struct ftitle_editor {
    std::map<double, unsigned char> actions;

    unsigned char playing;
};