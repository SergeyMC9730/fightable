#include <fightable/http/SplitString.hpp>
#include <string.h>

std::vector<std::string> splitString(const char* str, char d) {
	std::vector<std::string> result;
	size_t len = strlen(str);

	std::string current = "";

	for (size_t i = 0; i < len; i++) {
		char c = str[i];

		if (c == d) {
			result.push_back(current);

			current = "";
		}
		else {
			current += c;
		}
	}

	result.push_back(current);

	return result;
}