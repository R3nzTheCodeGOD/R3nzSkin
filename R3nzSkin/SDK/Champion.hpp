#pragma once

#include "AString.hpp"
#include "Pad.hpp"
#include "Skin.hpp"
#include "SkinArray.hpp"

class Champion {
	PAD(0x4);
public:
	AString champion_name;
private:
	PAD(0x48);
public:
	SkinArray<Skin> skins;
};