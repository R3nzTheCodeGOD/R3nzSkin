#pragma once

#include <cstdint>
#include <vector>

#include "AString.hpp"
#include "Pad.hpp"
#include "Skin.hpp"

class Champion {
	PAD(0x4);
public:
	AString champion_name;
private:
	PAD(0x48);
public:
	std::vector<Skin> skins;
};