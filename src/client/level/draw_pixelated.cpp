#include <fightable/level.h>
#include <fightable/block_library.h>
#include <fightable/tilemap.h>
#include <fightable/block.h>
#include <map>
#include <vector>

void _fLevelDrawPixelated(struct flevel* level, IRectangle frame) {
	if (!level || (frame.width * frame.height) <= 0 || !level->objects || level->data_size == 0) return;

	int tx = level->tilemap->tile_size.x;
	int ty = level->tilemap->tile_size.y;

	BeginScissorMode(frame.x, frame.y, frame.width, frame.height);
	Camera2D cam = level->camera;
	cam.target.x /= tx;
	cam.target.y /= ty;

	BeginMode2D(cam);
	for (unsigned int i = 0; i < level->data_size; i++) {
		struct fblock obj = level->objects[i];

		unsigned short id = _fBlockIdFromRenderable(obj.base);
		if (id == BLOCK_AIR) continue;

		Color col = YELLOW;
		switch (id) {
		case 1: case 2: case 3: case 4: case 5: case 6:
		case 7: case 8: case 9: case 10: case 11: case 12:
		case 13: case 14: case 15: case 16: case 17: case 18: {
			col = LIME;
			break;
		}
		case 19: case 20: case 21: case 22: case 23: case 24:
		case 25: case 26: case 27: case 28: case 29: case 30:
		case 31: case 32: case 33: case 34: case 35: {
			col = BROWN;
			break;
		}
		case 36: case 47: {
			col = RED;
			break;
		}
		case 37: {
			col = (Color){ 34, 32, 32, 255 };
			break;
		}
		case 38: {
			col = BLACK;
			break;
		}
		case 39: {
			col = WHITE;
			break;
		}
		case 40: case 41: case 42: case 43:
		case 44: case 45: case 46: case 69: {
			col = GRAY;
			break;
		}
		case 48: case 49: case 50: case 51:
		case 52: case 53: case 54: {
			col = DARKGRAY;
			break;
		}
		case 55: case 56: case 57: {
			col = BEIGE;
			break;
		}
		case 58: case 59: case 60: case 61:
		case 62: case 63: case 64: case 65:
		case 66: {
			col = LIGHTGRAY;
			break;
		}
		case 67: case 68: {
			col = GREEN;
			break;
		}
		case 70: case 71: {
			col = YELLOW;
			break;
		} 
		}

		int _x = frame.x + obj.base.block_x;
		int _y = frame.y + obj.base.block_y;

		DrawPixel(_x, _y, col);
	}
	EndMode2D();
	EndScissorMode();
}