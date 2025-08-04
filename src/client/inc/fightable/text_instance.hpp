
//          Sergei Baigerov 2024 - 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <fightable/text_instance.h>
#include <vector>
#include <string>

struct ftext_instance_entry {
    std::string text;

    Texture2D prerendered_text;

    Color color_a;
    Color color_b;

    enum ftext_instance_command command;
};

struct ftext_instance {
    std::vector<ftext_instance_entry> text_elements;
    std::string original_string;

    RLFont applicable_raylib_font;
    float raylib_size;
    float raylib_spacing;
    struct ftext_manager *applicable_fightable_font;

    enum ftext_instance_backend used_font_backend;
};
