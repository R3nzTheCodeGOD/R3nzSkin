#pragma once

#include <Windows.h>
#include <cstdint>
#include <chrono>

#include "GameClasses.hpp"

using namespace std::chrono_literals;

namespace Memory {
	void Search(bool gameClient = true) noexcept;

	std::uintptr_t getLeagueModule() noexcept { return reinterpret_cast<std::uintptr_t>(::GetModuleHandleA(nullptr)); }
	GameClient* getClient() noexcept { return *reinterpret_cast<GameClient**>(getLeagueModule() + offsets::global::GameClient); }
	AIBaseCommon* getLocalPlayer() noexcept { return *reinterpret_cast<AIBaseCommon**>(getLeagueModule() + offsets::global::Player); }
};
