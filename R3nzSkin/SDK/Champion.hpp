#pragma once

#include <cstdint>
#include <vector>

#include "AString.hpp"
#include "Pad.hpp"
#include "Skin.hpp"

class Champion {
public:
	PAD(0x4);
	AString champion_name;
	PAD(0x48);
	std::vector<Skin> skins;
};