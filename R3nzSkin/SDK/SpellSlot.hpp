#pragma once

#include <cstdint>

#include "Pad.hpp" 

class SpellSlot {
	PAD(0x1C);
public:
	std::int32_t level;
private:
	PAD(0x4);
public:
	float time;
};
