#pragma once

#include <Windows.h>
#include <chrono>
#include <cstdint>

#include "GameClasses.hpp"
#include "Offsets.hpp"

using namespace std::chrono_literals;

namespace Memory {
	void Search(bool gameClient = true) noexcept;

	__forceinline auto getLeagueModule() noexcept
	{
		return reinterpret_cast<std::uintptr_t>(::GetModuleHandleA(nullptr));
	}
	
	__forceinline auto getClient() noexcept
	{
		return *reinterpret_cast<GameClient**>(getLeagueModule() + offsets::global::GameClient);
	}
	
	__forceinline auto getLocalPlayer() noexcept
	{
		return *reinterpret_cast<AIBaseCommon**>(getLeagueModule() + offsets::global::Player);
	}

	__forceinline auto getHeroes() noexcept
	{
		return *reinterpret_cast<ManagerTemplate<AIHero>**>(getLeagueModule() + offsets::global::ManagerTemplate_AIHero_);
	}

	__forceinline auto getMinions() noexcept
	{
		return *reinterpret_cast<ManagerTemplate<AIMinionClient>**>(getLeagueModule() + offsets::global::ManagerTemplate_AIMinionClient_);
	}

	__forceinline auto getRiotWindow() noexcept
	{
		return *reinterpret_cast<HWND*>(getLeagueModule() + offsets::global::Riot__g_window);
	}
};
