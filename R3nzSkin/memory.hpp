#pragma once

#include <Windows.h>
#include <chrono>
#include <cstdint>

#include "GameClasses.hpp"
#include "Offsets.hpp"

using namespace std::chrono_literals;

namespace Memory {
	void Search(bool gameClient = true) noexcept;

	std::uintptr_t getLeagueModule() noexcept
	{
		return reinterpret_cast<std::uintptr_t>(::GetModuleHandleA(nullptr));
	}
	
	GameClient* getClient() noexcept
	{
		return *reinterpret_cast<GameClient**>(getLeagueModule() + offsets::global::GameClient);
	}
	
	AIBaseCommon* getLocalPlayer() noexcept
	{
		return *reinterpret_cast<AIBaseCommon**>(getLeagueModule() + offsets::global::Player);
	}

	ManagerTemplate<AIHero>* getHeroes() noexcept
	{
		return *reinterpret_cast<ManagerTemplate<AIHero>**>(getLeagueModule() + offsets::global::ManagerTemplate_AIHero_);
	}

	ManagerTemplate<AIMinionClient>* getMinions() noexcept
	{
		return *reinterpret_cast<ManagerTemplate<AIMinionClient>**>(getLeagueModule() + offsets::global::ManagerTemplate_AIMinionClient_);
	}
};
