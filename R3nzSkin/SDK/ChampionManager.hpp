#pragma once

#include <vector>

#include "Champion.hpp"
#include "Pad.hpp"

class ChampionManager {
	PAD(0x18)
	std::vector<Champion*> champions;
};
