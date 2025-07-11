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

#include <nt5emul/tui/progress_bar.h>
#include <nt5emul/tui/rectangle.h>

void _ntTuiDrawProgressBar(float percentage, RLRectangle position, Color color_bg, Color color_fg) {
    // boundary check
    if (percentage > 100.f) {
        percentage = 100.f;
    } else if (percentage < 0.f) {
        percentage = 0.f;
    }

    // draw background rectangle
    _ntTuiDrawRectangle(position, color_bg);

    // calculate size for a foreground rectangle
    position.width *= (percentage / 100.f);

    // draw rectangle
    _ntTuiDrawRectangle(position, color_fg);
}
