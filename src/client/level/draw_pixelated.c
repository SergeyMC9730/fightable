
//          Sergei Baigerov 2024 - 2026.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include "fightable/intrect.h"
#include <fightable/level.h>
#include <fightable/block_library.h>
#include <fightable/tilemap.h>
#include <fightable/block.h>
#include <fightable/renderer.h>

struct fLDPResult _fLevelDrawPixelated(struct flevel* level, IRectangle frame) {
    if (!level || (frame.width * frame.height) <= 0 || !level->objects || level->data_size == 0) return (struct fLDPResult){};

    int tx = level->tilemap->tile_size.x;
    int ty = level->tilemap->tile_size.y;

    unsigned char found_startpos = 0;
    IVector2 line_origin = {};

    Camera2D cam = level->camera;
    for (unsigned int i = 0; i < level->data_size; i++) {
        struct fblock obj = level->objects[i];

        unsigned short id = _fBlockIdFromRenderable(obj.base);
        if (id == BLOCK_AIR) continue;

        int _x = frame.x + obj.base.block_x - (int)cam.target.x;
        int _y = frame.y + obj.base.block_y - (int)cam.target.y;

        if (id == BLOCK_START) {
            found_startpos = 1;
            line_origin = (IVector2){_x, _y};
        }

        if ((_x < frame.x || _x > frame.x + frame.width - 1) || (_y < frame.y || _y  > frame.y + frame.height - 1)) continue;

        Color col = YELLOW;
        switch (id) {
        case 1: case 2: case 3: case 4: case 5: case 6:
        case 7: case 8: case 9: case 10: case 11: case 12:
        case 13: case 14: case 15: case 16: case 17: case 18:
        case 72: case 73: case 74: case 75: case 76: case 77:
        case 78: case 85: {
            col = LIME;
            break;
        }
        case 19: case 20: case 21: case 22: case 23: case 24:
        case 25: case 26: case 27: case 28: case 29: case 30:
        case 31: case 32: case 33: case 34: case 35: case 79:
        case 80: case 81: case 82: case 83: case 84: case 119: {
            col = BROWN;
            break;
        }
        case 36: case 47: case 106: case 108: {
            col = RED;
            break;
        }
        case 37: {
            col = (Color){ 34, 32, 32, 255 };
            break;
        }
        case 38: case 118: {
            col = BLACK;
            break;
        }
        case 39: case 107: {
            col = WHITE;
            break;
        }
        case 40: case 41: case 42: case 43:
        case 44: case 45: case 46: case 69:
        case 117: {
            col = GRAY;
            break;
        }
        case 48: case 49: case 50: case 51:
        case 52: case 53: case 54: case 90:
        case 91: case 92: case 93: case 94:
        case 95: case 96: case 97: case 98:
        case 99: case 100: case 101: case 102:
        case 103: case 104: case 105: {
            col = DARKGRAY;
            break;
        }
        case 55: case 56: case 57: case 86:
        case 87: case 88: case 89: {
            col = BEIGE;
            break;
        }
        case 58: case 59: case 60: case 61:
        case 62: case 63: case 64: case 65:
        case 66: {
            col = LIGHTGRAY;
            break;
        }
        case 67: case 68: case 111: {
            col = GREEN;
            break;
        }
        case 70: case 71: case 109: {
            col = YELLOW;
            break;
        }
        case 110: {
            col = GOLD;
            break;
        }
        case 112: {
            col = SKYBLUE;
            break;
        }
        case 113: {
            col = BLUE;
            break;
        }
        case 114: {
            col = VIOLET;
            break;
        }
        case 115: case 116: {
            col = PINK;
            break;
        }
        }

        DrawPixel(_x, _y, col);
    }

    if (found_startpos) {
        Color c1 = BLACK;
        c1.a = 32;
        //DrawRectangleGradientV(line_origin.x, line_origin.y, line_origin.x - frame.x, 1, c1, c2);
        //DrawRectangleGradientV(line_origin.x, line_origin.y, 1, line_origin.y - frame.y, c1, c2);
        DrawLine(frame.x, frame.y, line_origin.x, line_origin.y, c1);
        DrawLine(frame.x + frame.width, frame.y, line_origin.x, line_origin.y, c1);
        DrawLine(frame.x, frame.y + frame.height, line_origin.x, line_origin.y, c1);
        DrawLine(frame.x + frame.width, frame.y + frame.height, line_origin.x, line_origin.y, c1);
    }

    return (struct fLDPResult){found_startpos};
}
