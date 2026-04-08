
//          Sergei Baigerov 2024 - 2026.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include <cstring>
#include <queue>
#include <fightable/translation.h>
#include <fightable/state.h>
#include <fightable/renderer.h>

struct ftr_queue {
    IVector2 pos;
    const char *entry_id;
    float scaling;
    Color tint;
    unsigned char should_convert;
};

std::queue<ftr_queue> __trQueue;
flang_id __trLang = FLI_RuRu;
std::vector<ftr_entry> __trStrings = {
    {
        "menu.base.play",
        "PLAY",
        "ИГРАТЬ"
    },
    {
        "menu.base.options",
        "OPTIONS",
        "НАСТРОЙКИ"
    },
    {
        "menu.base.editor",
        "EDITOR",
        "РЕДАКТОР"
    },
    {
        "menu.base.exit",
        "EXIT",
        "ВЫЙТИ"
    },
    {
        "editor.save",
        "Save",
        "СОХРАНИТЬ"
    },
    {
        "editor.edit",
        "Edit",
        "РЕДАКТ."
    },
    {
        "editor.swipe",
        "Swipe",
        "СВАЙП"
    },
    {
        "editor.more",
        "More",
        "ДАЛЕЕ"
    },
    {
        "editor.perlin_gen",
        "Perlin Gen",
        "ШУМ ПЕРЛИНА"
    },
    {
        "editor.blocks",
        "Blocks",
        "БЛОКИ"
    },
    {
        "editor.play",
        "Play",
        "ВОЙТИ"
    },
    {
        "editor.play.exit",
        "Exit",
        "ВЫЙТИ"
    },
    {
        "editor.play.f1",
        "F1 to exit",
        "Для выхода нажмите F1"
    },
    {
        "editor.back",
        "Back",
        "НАЗАД"
    },
    {
        "menu.mp.join",
        "Join",
        "ВОЙТИ"
    },
    {
        "menu.mp.exit",
        "Exit",
        "ВЫЙТИ"
    },
    {
        "menu.play.back",
        "BACK",
        "НАЗАД"
    },
    {
        "menu.options.back",
        "BACK",
        "НАЗАД"
    },
    {
        "mp.lobby.open",
        "Open",
        "ЗАГРУЗИТЬ"
    },
    {
        "mp.lobby.join",
        "Join",
        "ВОЙТИ"
    },
    {
        "notification.song",
        "Playing %s",
        "Играет «%s»"
    },
    {
        "notification.mpc.disconnected",
        "Disconnected\nfrom server",
        "Отключён\nот сервера"
    },
    {
        "notification.mpc.connected",
        "<cgreen,white>Connection\n<cgreen,white>established",
        "<cgreen,white>Подключение\n<cgreen,white>выполнено успешно"
    },
    {
        "notification.mpc.assert",
        "<cred,orange>ERROR:\n<cyellow>Pre-connect assertion failed",
        "<cred,orange>ОШИБКА:\n<cyellow>Предварительный набор\nданных не прошёл проверку"
    },
    {
        "notification.mpc.basic_error",
        "<cred,orange>ERROR:\n<cyellow>Connection failed",
        "<cred,orange>ОШИБКА:\n<cyellow>Не удалось установить\nсоединение с сервером"
    },
    {
        "notification.mpc.download",
        "<cgreen,white>Downloading\n<cgreen,white>%s",
        "<cgreen,white>Скачивается\n<cgreen,white>%s"
    },
    {
        "notification.editor.saved",
        "Saved under ",
        "Уровень сохранён под именем\n"
    },
    {
        "notification.funny.remember",
        "* human.. i remember you're\n  <cred,orange>genocides..\n ",
        "* человек.. помню, ты когда-то..\n  <cred,orange>геноцидил..\n "
    },
    {
        "notification.level.load_failed",
        "Could not open level",
        "Не удалось открыть уровень"
    },
    {
        "notification.intro.load_failed",
        "Could not initialize intro properly",
        "Не удалось загрузить заставку"
    },
    {
        "notification.mp.no_ip",
        "<cred,orange>ERROR:\n<cyellow>IP address is empty!",
        "<cred,orange>ОШИБКА:\n<cyellow>Поле ввода IP-адреса не заполнено!"
    },
    {
        "notification.mp.no_username",
        "<cred,orange>ERROR:\n<cyellow>Username field is empty!",
        "<cred,orange>ОШИБКА:\n<cyellow>Поле ввода никнейма пустое!"
    },
    {
        "notification.mp.connecting",
        "<cyellow>Connecting to the server...",
        "<cyellow>Подключение к серверу..."
    },
    {
        "notification.mp.connecting",
        "<cyellow>Connecting to the server...",
        "<cyellow>Подключение к серверу..."
    },
    {
        "menu.play.create_new",
        "Create New",
        "Открыть сервер"
    },
    {
        "menu.play.singleplayer",
        "Singleplayer",
        "Одиночная игра"
    },
    {
        "menu.play.join_server",
        "Join Server",
        "Подключиться\nк серверу"
    }
};

void _fTranslationQueueTextDraw(IVector2 pos, const char *entry_id, float scaling, Color tint, unsigned char should_convert) {
    ftr_queue q = {pos, entry_id, scaling, tint, should_convert};
    __trQueue.push(q);
}

void _fTranslationRender() {
    Vector2 offset = _fPosFramebufferToOverlay(__state.gui_render_offset);

    for (; !__trQueue.empty(); __trQueue.pop()) {
        auto q = __trQueue.front();

        const char *lang_str = _fTranslationGetString(q.entry_id);

        auto new_pos = _fImathToVFloat(q.pos);
        if (q.should_convert) {
            new_pos = _fPosFramebufferToOverlay(new_pos);
        }
        new_pos.x -= offset.x;
        new_pos.y -= offset.y;
        // TraceLog(LOG_INFO, "%s | %f | %f %f | %f",  lang_str, 16.f * q.scaling,new_pos.x, new_pos.y,q.scaling);
        RlDrawTextEx(__state.ubuntu_mono64, lang_str, new_pos, 64.f * q.scaling, 0.5f, q.tint);
    }
}

enum flang_id _fTranslationGetCurrentLanguage() {
    return __trLang;
}

const char *_fTranslationGetString(const char *entry_id) {
    const char *lang_str = NULL;
    for (const auto &e : __trStrings) {
        if (!strcmp(e.entry_id, entry_id)) {
            switch (__trLang) {
                case flang_id::FLI_EnUs: {
                    lang_str = e.en_us;
                    break;
                }
                case flang_id::FLI_RuRu: {
                    lang_str = e.ru_ru;
                    break;
                }
                default: {
                    lang_str = entry_id;
                    break;
                }
            }
        }
    }
    if (!lang_str) {
        lang_str = entry_id;
    }

    return lang_str;
}
