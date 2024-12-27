#include <fightable/intro.h>
#include <fightable/sound_library.h>
#include <fightable/state.h>

#include <array>
#include <string>
#include <vector>

class fintro_shaker_base {
public:
    virtual void update() = 0;
};

template <int rows_to_check>
class fintro_shaker : public fintro_shaker_base {
protected:
    std::array<bool, rows_to_check> _shake_lock = {};
    std::vector<std::string> _valid_instruments = {};
    int _check_offset = 0;
public:
    fintro_shaker(std::vector<std::string> valid_instruments, int check_offset = 0) {
        _valid_instruments = valid_instruments;
        _check_offset = check_offset;

        _shake_lock.fill(false);
    }

    std::string get_shake_locks() {
        std::string res = "";

        for (int i = 0; i < _shake_lock.size(); i++) {
            res += std::to_string(_shake_lock[i]) + " ";
        }

        if (res.size() > 0) {
            res.pop_back();
        }

        return res;
    }

    void update() override {
        // std::string rows = get_shake_locks();

        // TraceLog(LOG_INFO, "rows: %s", rows.c_str());

        for (int i = _check_offset; i < _shake_lock.size() + _check_offset; i++) {
            const char* rowc = _fAudioGetDbg(&__state.sound_engine, i);
            
            if (!rowc) {
                TraceLog(LOG_ERROR, "pattern row at %d is null!", i);
                continue;
            }

            std::string row = rowc;
            free((void *)rowc);

            bool should_shake = false;

            for (std::string& instrument : _valid_instruments) {
                should_shake |= (row.find(instrument) != std::string::npos);
            }

            if (should_shake) {
                if (!_shake_lock[i]) {
                    if (!__state.sound_engine.do_not_shake) {
                        _fGfxShake(&__state.gfx, 1.f);
                    }
                    _shake_lock[i] = 1;
                }
            }
            else {
                _shake_lock[i] = 0;
            }
        }
    }
};

class fintro_ed0_shaker : public fintro_shaker<1> {
public:
    fintro_ed0_shaker() : fintro_shaker({ "0D", "14" }, 5) {}
};
class fintro_ed1_shaker : public fintro_shaker<4> {
public:
    fintro_ed1_shaker() : fintro_shaker({ "08" }, 0) {}
};

static fintro_shaker_base* __current_shaker = nullptr;

void _fIntroProcessGfx() {
    if (__current_shaker != nullptr) {
        __current_shaker->update();
    }
}

void _fIntroGfxInit() {
#if defined(TARGET_UNIX) && !defined(TARGET_LINUX)
    #pragma message("illegal instruction while executing shake code. ignoring this effect")
    return;
#endif

    switch (__state.song_id) {
    case MUS_ENDLESS_DREAM: {
        __current_shaker = new fintro_ed0_shaker();
        break;
    }
    case MUS_ELECTRIC_DRUG: {
        __current_shaker = new fintro_ed1_shaker();
        break;
    }
    }
}