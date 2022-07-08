#pragma once

#include "AString.hpp"
#include "Pad.hpp"
#include "Skin.hpp"
#include "SkinArray.hpp"

class Champion {
	PAD(0x4)
	AString champion_name;
	PAD(0x48)
	SkinArray<Skin> skins;
};