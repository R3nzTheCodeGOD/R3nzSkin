#pragma once

#include <cstdint>

#include "AString.hpp"

class Skin {
public:
	std::int32_t skin_id;
	std::int32_t unknown; // always 0x1f4
	AString skin_name;
};
