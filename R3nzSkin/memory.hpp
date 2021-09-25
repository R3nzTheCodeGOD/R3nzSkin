#pragma once

#include <Windows.h>
#include <chrono>
#include <cstdint>

#include "GameClasses.hpp"
#include "Offsets.hpp"

using namespace std::chrono_literals;

namespace Memory {
	void Search(bool gameClient = true) noexcept;

	__forceinline std::uintptr_t getLeagueModule() noexcept
	{
		return reinterpret_cast<std::uintptr_t>(::GetModuleHandleA(nullptr));
	}
	
	__forceinline GameClient* getClient() noexcept
	{
		return *reinterpret_cast<GameClient**>(getLeagueModule() + offsets::global::GameClient);
	}
	
	__forceinline AIBaseCommon* getLocalPlayer() noexcept
	{
		return *reinterpret_cast<AIBaseCommon**>(getLeagueModule() + offsets::global::Player);
	}

	__forceinline ManagerTemplate<AIHero>* getHeroes() noexcept
	{
		return *reinterpret_cast<ManagerTemplate<AIHero>**>(getLeagueModule() + offsets::global::ManagerTemplate_AIHero_);
	}

	__forceinline ManagerTemplate<AIMinionClient>* getMinions() noexcept
	{
		return *reinterpret_cast<ManagerTemplate<AIMinionClient>**>(getLeagueModule() + offsets::global::ManagerTemplate_AIMinionClient_);
	}

	__forceinline HWND getRiotWindow() noexcept
	{
		return *reinterpret_cast<HWND*>(getLeagueModule() + offsets::global::Riot__g_window);
	}
};
