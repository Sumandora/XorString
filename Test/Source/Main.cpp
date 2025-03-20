#include "XorString.hpp"

#include <cassert>
#include <cstring>
#include <string>

int main()
{
	assert(std::strcmp(XORSTR("ABC"), "ABC") == 0);
	assert(std::string{ XORSTR("ABC") } == "ABC");

	return 0;
}
