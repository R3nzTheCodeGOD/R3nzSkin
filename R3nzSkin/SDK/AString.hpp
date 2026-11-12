#pragma once

#include <cstdint>

class AString {
public:
	const char* str;
	std::int32_t length;
	std::int32_t capacity;

	// Game-owned string: capacity includes ownership flags, not an STL capacity.
	[[nodiscard]] const char* c_str() const noexcept { return str ? str : ""; }
};

static_assert(sizeof(AString) == 0x10, "AString requires the x64 game layout");
