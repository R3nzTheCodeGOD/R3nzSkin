#pragma once

#include "GameState.hpp"
#include "Pad.hpp"

class GameClient {
public:
	PAD(0x8);
	GGameState_s game_state;
};
