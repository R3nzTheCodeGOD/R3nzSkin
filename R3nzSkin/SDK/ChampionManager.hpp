#pragma once

#include <vector>

#include "Champion.hpp"
#include "Pad.hpp"
#include "RiotArray.hpp"

class ChampionManager {
	PAD(0x18)
	RiotArray<Champion*> champions;
};
