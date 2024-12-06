#include <fightable/generic_tools.hpp>

std::vector<std::string> GenericTools::splitString(const char* str, char d, unsigned int max_entries) {
    std::vector<std::string> result;

    do {
        const char* begin = str;

        while (*str != d && *str) str++;

        std::string _ready = { begin, str };
        if (!_ready.empty()) {
            result.push_back(_ready);
        }

        if (max_entries > 0 && result.size() > max_entries) {
            break;
        }
    } while (0 != *str++);

    char delim_cstr[2] = { d, 0 };
    std::string delim_str = delim_cstr;

    int sz = result.size();

    if (sz != 0) {
        if (stringEndsWith(result[sz - 1], delim_str)) {
            result[sz - 1].pop_back();
        }
    }

    return result;
}

#include <cstring>

bool GenericTools::stringEndsWith(const std::string &input, const std::string &substring) {
    return !std::strncmp(input.c_str() + input.size() - substring.size(), substring.c_str(), substring.size());
}