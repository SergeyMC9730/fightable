/*
    nt5 -- Windows XP simulator.
    Copyright (C) 2023  SergeyMC9730

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU Affero General Public License as published
    by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.

    Contact SergeyMC9730 -- @dogotrigger in Discord
*/

#pragma once

#include <fraylib.h>

struct nt_tui_environment {
    RLFont font;

    // size in characters
    Vector2 base_font_size;

    bool show_input_pointer;

    bool exists;

    float scaling;
};

#ifdef __cplusplus
extern "C" {
#endif

// load text ui environment
void _ntTuiLoadEnvironment(const char *font_path, Vector2 base_font_size, float scaling);

// load text ui environment with default parameters
void _ntTuiLoadEnvironmentDefault(float scaling);

struct nt_tui_environment *_ntGetTuiEnvironment();

#ifdef __cplusplus
}
#endif
