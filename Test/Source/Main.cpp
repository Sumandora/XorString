#include "XorString.hpp"
#include <cstring>
#include <string>
#include <cassert>

int main() {
	assert(strcmp(XORSTR("ABC"), "ABC") == 0);
	assert(std::string{ XORSTR("ABC") } == "ABC");

	return 0;
}
