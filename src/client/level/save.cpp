#include <fightable/level.h>
#include <fightable/generic_tools.hpp>
#include <fightable/block.h>
#include <fightable/block_library.h>

void _fLevelSave(struct flevel* level, const char* filename) {
	if (!level || !level->objects || !filename) return;

	std::vector<uint8_t> output;
	unsigned int valid_blocks = 0;

	for (unsigned int i = 0; i < level->data_size; i++) {
		struct fblock block = level->objects[i];

		if (_fBlockIdFromBlock(block) == BLOCK_AIR) continue;
		valid_blocks++;
	}

	GenericTools::addVectors(&output, GenericTools::valueToVector(LEVEL_FORMAT_VERSION));
	GenericTools::addVectors(&output, GenericTools::valueToVector(level->width));
	GenericTools::addVectors(&output, GenericTools::valueToVector(level->height));
	GenericTools::addVectors(&output, GenericTools::valueToVector(valid_blocks));

	for (unsigned int i = 0; i < level->data_size; i++) {
		struct fblock block = level->objects[i];
		unsigned short id = _fBlockIdFromBlock(block);

		if (id == BLOCK_AIR) continue;

		uint8_t bitdata = 0;
		bitdata |= (block.base.flipped_x << 0);
		bitdata |= (block.base.flipped_y << 1);
		bitdata |= (block.is_start_pos << 2);
		bitdata |= (block.passable << 3);
		bitdata |= (block.dangerous << 4);

		GenericTools::addVectors(&output, GenericTools::valueToVector(id)); // ->2
		GenericTools::addVectors(&output, GenericTools::valueToVector(block.base.block_x)); // ->4
		GenericTools::addVectors(&output, GenericTools::valueToVector(block.base.block_y)); // ->6
		GenericTools::addVectors(&output, GenericTools::valueToVector(block.group_id)); // ->8
		GenericTools::addVectors(&output, GenericTools::valueToVector(bitdata)); // -> 10
	}

	TraceLog(LOG_INFO, "Output size: %d bytes", output.size());

	if (output.size() == 0) return;
	SaveFileData(filename, output.data(), output.size());
}