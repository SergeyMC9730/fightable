#include "raylib.h"
#include <fightable/level.h>
#include <fightable/block.h>
#include <fightable/block_library.h>
#include <fightable/state.h>

struct flevel* _fLevelLoadFromFile(const char* filename) {
	int len = 0;
	unsigned char* data = LoadFileData(filename, &len);

	if (!data) return NULL;
	if (len < 10) {
		MemFree(data);
		return NULL;
	}

	short format = ((short*)data)[0];
	TraceLog(LOG_INFO, "Level format: %d", format);

	struct flevel* level = NULL;

	switch (format) {
	case 2: {
		TraceLog(LOG_INFO, "Loading outdated level");

		unsigned short width = ((short*)data)[1];
		unsigned short height = ((short*)data)[2];

		TraceLog(LOG_INFO, "Level size: %dx%d", width, height);

		unsigned int objects = *(unsigned int*)(data + 6) % 3000000;

		TraceLog(LOG_INFO, "Objects: %d", objects);

		level = (struct flevel*)MemAlloc(sizeof(struct flevel));
		*level = _fLevelLoadTest(__state.tilemap, (IVector2) { 28, 4 });

		free(level->objects);

		level->width = width;
		level->height = height;
		level->data_size = objects;
		level->objects = (struct fblock*)MemAlloc(sizeof(struct fblock) * level->data_size);

		for (unsigned int i = 0; i < objects; i++) {
			unsigned int offset = 10 + (9 * i);
			if (offset >= len) break;

			unsigned char* ref = data + offset;
			short* sref = (short*)ref;

			unsigned short id = sref[0];
			if (id == BLOCK_AIR || id == INVALID_BLOCK_ID) {
				// TraceLog(LOG_INFO, "Invalid block id %d at pos %d(%d)", (int)ref, i);
				continue;
			}

			short x = sref[1];
			short y = sref[2];
			unsigned short gid = sref[3];

#pragma message("should we really handle bitfield if its already set? check line 58")
			// uint8_t bitdata = ref[9];

			struct fblock block = _fBlockFromId(id);
			block.group_id = gid;
			block.layer_id = 0;
			block.base.block_x = x;
			block.base.block_y = y;

			level->objects[i] = block;
		}

		break;
	}
	case LEVEL_FORMAT_VERSION: {
		unsigned short width = ((short*)data)[1];
		unsigned short height = ((short*)data)[2];

		TraceLog(LOG_INFO, "Level size: %dx%d", width, height);

		unsigned int objects = *(unsigned int*)(data + 6) % 3000000;

		TraceLog(LOG_INFO, "Objects: %d", objects);

		level = (struct flevel*)MemAlloc(sizeof(struct flevel));
		*level = _fLevelLoadTest(__state.tilemap, (IVector2) { 28, 4 });

		free(level->objects);

		level->width = width;
		level->height = height;
		level->data_size = objects;
		level->objects = (struct fblock*)MemAlloc(sizeof(struct fblock) * level->data_size);

		for (unsigned int i = 0; i < objects; i++) {
			unsigned int offset = 10 + (11 * i);
			if (offset >= len) break;

			unsigned char* ref = data + offset;
			short* sref = (short*)ref;

			unsigned short id = sref[0];
			if (id == BLOCK_AIR || id == INVALID_BLOCK_ID) {
				// TraceLog(LOG_INFO, "Invalid block id %d at pos %d(%d)", (int)ref, i);
				continue;
			}

			short x = sref[1];
			short y = sref[2];
			unsigned short gid = sref[3];
			unsigned short lid = sref[4];

#pragma message("should we really handle bitfield if its already set? check line 110")
			// uint8_t bitdata = ref[11];

			struct fblock block = _fBlockFromId(id);
			block.group_id = gid;
			block.layer_id = lid;
			block.base.block_x = x;
			block.base.block_y = y;

			level->objects[i] = block;
		}

		break;
	}
	}

	MemFree(data);

	return level;
}
