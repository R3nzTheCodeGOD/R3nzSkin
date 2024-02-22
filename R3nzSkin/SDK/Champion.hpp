#pragma once

#include "AString.hpp"
#include "Pad.hpp"
#include "Skin.hpp"
#include "RiotArray.hpp"

class Champion {
	PAD(0x8)
	AString champion_name;
	PAD(0xA0)
	RiotArray<Skin> skins;
};
