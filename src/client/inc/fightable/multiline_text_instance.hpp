
//          Sergei Baigerov 2024 - 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <fightable/multiline_text_instance.h>
#include <vector>
#include <string>

struct fmultiline_text_instance {
    std::vector<struct fmultiline_text_instance_entry> lines;
    std::string original_string;
};
