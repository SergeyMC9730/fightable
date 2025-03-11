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
#ifdef TARGET_BIG_ENDIAN
	GenericTools::addVectors(&output, GenericTools::valueToVector((unsigned char)1));
#else
	GenericTools::addVectors(&output, GenericTools::valueToVector((unsigned char)0));
#endif

	for (unsigned int i = 0; i < level->data_size; i++) {
		struct fblock block = level->objects[i];
		unsigned short id = _fBlockIdFromBlock(block);

		if (id == BLOCK_AIR) continue;

		GenericTools::addVectors(&output, GenericTools::valueToVector(id));
		GenericTools::addVectors(&output, GenericTools::valueToVector(block.base.block_x));
		GenericTools::addVectors(&output, GenericTools::valueToVector(block.base.block_y));
		GenericTools::addVectors(&output, GenericTools::valueToVector(block.layer_id));
		GenericTools::addVectors(&output, GenericTools::valueToVector(block.registry_id));
		GenericTools::addVectors(&output, GenericTools::valueToVector(_fBlockGetBitfield(&block)));
	}

	if (level->block_entries == NULL) {
		GenericTools::addVectors(&output, GenericTools::valueToVector((unsigned int)(0)));
	}
	else {
		GenericTools::addVectors(&output, GenericTools::valueToVector(level->block_entries->len));

		TraceLog(LOG_INFO, "Amount of block entries: %d", level->block_entries->len);

		for (unsigned int i = 0; i < level->block_entries->len; i++) {
			auto e = level->block_entries->objects[i];
			GenericTools::addVectors(&output, GenericTools::valueToVector(e.id));

			unsigned int l = 0;
			unitype_t* u = e.entry;

			while (u != NULL) {
				u = u->next;
				l++;
			}

			GenericTools::addVectors(&output, GenericTools::valueToVector(l));

			TraceLog(LOG_INFO, "%d: Amount of extra objects: %d", i, l);

			if (l != 0) {
				u = e.entry;

				while (u != NULL) {
				    if (!u->name || (u->p == NULL && u->p_sz != 0) || (u->p != NULL && u->p_sz == 0)) {
						TraceLog(LOG_WARNING, "%d: Entry cannot be saved properly %s(status=%d%d%d%d%d|%p.%d.%c)",
						    i,
						    u->next == NULL ? "and continued " : "\0",
							!u->name,
							u->p == NULL,
							u->p_sz != 0,
							u->p != NULL,
							u->p_sz == 0,
							u->p,
							u->p_sz,
							u->type
						);

						GenericTools::addVectors(&output, GenericTools::valueToVector((size_t)0));
						GenericTools::addVectors(&output, GenericTools::valueToVector(u->type));
						GenericTools::addVectors(&output, GenericTools::valueToVector(u->p_sz));

						if (u->next == NULL) {
							break;
						} else {
							u = u->next;
							continue;
						}
					}

					std::string name = u->name;
					size_t name_len = name.length();

					GenericTools::addVectors(&output, GenericTools::valueToVector(name_len));
					GenericTools::addVectors(&output, GenericTools::stringToVector<uint8_t>(name));

					GenericTools::addVectors(&output, GenericTools::valueToVector(u->type));
					GenericTools::addVectors(&output, GenericTools::valueToVector(u->p_sz));
					GenericTools::addVectors(&output, GenericTools::arrayToVector((uint8_t *)u->p, u->p_sz));

					u = u->next;
				}
			}
		}
	}

	TraceLog(LOG_INFO, "Output size: %d bytes", output.size());

	if (output.size() == 0) return;
	SaveFileData(filename, output.data(), output.size());
}
