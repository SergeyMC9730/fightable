
//          Sergei Baigerov 2024 - 2026.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

struct ftr_entry {
    const char *entry_id;

    const char *en_us;
    const char *ru_ru;
};

enum flang_id {
    FLI_EnUs,
    FLI_RuRu
};

#include <fightable/intvec.h>

#ifdef __cplusplus
extern "C" {
#endif

void _fTranslationRender();

// translated string is drawn on overlay instead because of 3x5 font restrictions
void _fTranslationQueueTextDraw(IVector2 pos, const char *entry_id, float scaling, Color tint, unsigned char should_convert);

const char *_fTranslationGetString(const char *entry_id);
enum flang_id _fTranslationGetCurrentLanguage();

#ifdef __cplusplus
}
#endif
