#pragma once

#include <cstdint>

enum class GGameState_s : std::int32_t {
	LoadingScreen = 0,
	Connecting = 1,
	Running = 2,
	Poaused = 3,
	Finished = 4,
	Exiting = 5
};
