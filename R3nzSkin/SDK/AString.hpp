#pragma once

#include <cstdint>

class AString {
public:
	const char* str;
	std::int32_t length;
	std::int32_t capacity;
};
