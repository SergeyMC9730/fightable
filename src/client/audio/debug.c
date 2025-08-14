
//          Sergei Baigerov 2024 - 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include <fightable/sound_engine.h>
#include <libopenmpt/libopenmpt.h>

const char *_fAudioGetChannelRow(struct faudio_engine *engine, int channel) {
    if (!engine->current_module) return NULL;

    return openmpt_module_format_pattern_row_channel(engine->current_module, engine->_pattern, engine->_row, channel, 0, 0);
}
