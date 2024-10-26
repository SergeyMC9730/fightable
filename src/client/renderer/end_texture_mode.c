/*
    nt5 -- Windows XP simulator.
    Copyright (C) 2023  Sergei Baigerov

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

    Contact Sergei Baigerov -- @dogotrigger in Discord
*/

#include <fightable/renderer.h>
#include <fightable/state.h>

void EndTextureModeStacked() {
    int current = __state.r2dpointer - 1;

    if (current == 0) {
        __state.r2dpointer--;
        
        return EndTextureMode();
    }
    
    RenderTexture2D target = __state.r2dstack[current - 1];

    EndTextureMode();
    BeginTextureMode(target);

    __state.r2dpointer--;
}