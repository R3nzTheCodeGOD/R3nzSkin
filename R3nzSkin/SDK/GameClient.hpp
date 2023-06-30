#pragma once

#include "GameState.hpp"
#include "Pad.hpp"

class GameClient {
	PAD(0xC)
	GGameState_s game_state;
};
