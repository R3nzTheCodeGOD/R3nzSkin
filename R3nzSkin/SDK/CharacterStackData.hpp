#pragma once

#include <cstdint>

#include "AString.hpp"
#include "Pad.hpp"

class CharacterStackData {
public:
	AString model;
	std::int32_t skin;
	PAD(0x20);
	bool update_spells;
	bool dont_update_hud;
	bool change_particle;
	PAD(0x1);
	PAD(0xC);
};
