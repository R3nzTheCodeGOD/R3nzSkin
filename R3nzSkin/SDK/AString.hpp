#pragma once

#include <cstddef>

class AString {
public:
	const char* str;
	std::size_t length;
	std::size_t capacity;
};
