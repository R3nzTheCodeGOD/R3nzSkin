#pragma once

#include "GameState.hpp"
#include "Pad.hpp"
#include "../offsets.hpp"
#include <intrin.h>

class GameClient {
	PAD(0x10)
	std::uint32_t encoded_game_state;

public:
	[[nodiscard]] GGameState_s get_game_state() const noexcept
	{
		return static_cast<GGameState_s>((_rotr(encoded_game_state, offsets::encoding::GameClientRotate) ^
			offsets::encoding::GameClientXor) + offsets::encoding::GameClientAdd);
	}
};
