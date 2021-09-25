#pragma once

#include <chrono>

#include "GameClasses.hpp"

using namespace std::chrono_literals;

namespace Memory {
	void Search(bool gameClient = true) noexcept;

	GameClient* getClient() noexcept { *reinterpret_cast<GameClient**>(std::uintptr_t(::GetModuleHandleA(nullptr)) + offsets::global::GameClient); }
};
