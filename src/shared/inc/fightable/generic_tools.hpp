#pragma once

#include <vector>
#include <string>
#include <cstdio>
namespace GenericTools {
    template<typename T>
    inline void addVectors(std::vector<T> *output, const std::vector<T> &input) {
        for (T &obj : input) {
            output->push_back(obj);
        }
    }

    #pragma message("should we handle cpu endianess?")
    
    template<typename T>
    inline std::vector<unsigned char> valueToVector(T* value) {
        unsigned int val_size = sizeof(T);
        unsigned char *bytes = (unsigned char *)value;

        std::vector<unsigned char> vec;

        for (unsigned int i = 0; i < val_size; i++) {
            vec.push_back(bytes[i]);
        }

        return vec;
    }

    template<typename T = char>
    inline std::vector<T> stringToVector(const std::string &str) {
        std::vector<T> v;

        for (char c : str) {
            v.push_back(c);
        }

        return v;
    }

    template<typename T>
    inline std::string valueToHex(const T &v) {
	constexpr size_t sz = sizeof(T);
	const auto work = (unsigned char *)&v;
	std::string result;
	
	for (unsigned int i = 0; i < sz; i++) {
	    const unsigned char b = work[i];
	    char buf[3];
	    
	    snprintf(buf, 3, "%02X", b);
	    result += (std::string)buf;
	}
	
	return result;
    }

    template<typename T>
    inline std::string vectorToString(const std::vector<T> &v) {
        std::string res = "";

        for (const T &el : v) {
            res += valueToHex<T>(el) + " ";
        }

        if (res.length() >= 1) {
            res.pop_back();
        }

        return res;
    }

    template<typename T>
    inline void deleteVec(const std::vector<T *> &vec) {
        for (T *el : vec) {
            delete el;
        }
    }

    template<typename T>
    inline T vectorToValue(const std::vector<unsigned char> &vec, unsigned int offset = 0) {
        T v;
        unsigned char *raw_ref = (unsigned char *)(&v);

        for (unsigned int i = 0; i < sizeof(T); i++) {
            raw_ref[i] = vec[i + offset];
        }

        return v;
    }

    std::vector<std::string> splitString(const char* str, char d, unsigned int max_entries = 0);

    bool stringEndsWith(const std::string &input, const std::string &substring);
};
