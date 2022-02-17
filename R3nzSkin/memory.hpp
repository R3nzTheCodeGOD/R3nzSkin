#pragma once

#include <Windows.h>
#include <cstdint>

#include "Offsets.hpp"
#include "SDK/AIBaseCommon.hpp"
#include "SDK/AIHero.hpp"
#include "SDK/AIMinionClient.hpp"
#include "SDK/Chat.hpp"
#include "SDK/GameClient.hpp"
#include "SDK/ManagerTemplate.hpp"

namespace Memory {
	void Search(bool gameClient = true) noexcept;

	[[nodiscard]] __forceinline auto getLeagueModule() noexcept { return reinterpret_cast<std::uintptr_t>(::GetModuleHandle(nullptr)); }
	[[nodiscard]] __forceinline auto getClient() noexcept { return *reinterpret_cast<GameClient**>(getLeagueModule() + offsets::global::GameClient); }
	[[nodiscard]] __forceinline auto getLocalPlayer() noexcept { return *reinterpret_cast<AIBaseCommon**>(getLeagueModule() + offsets::global::Player); }
	[[nodiscard]] __forceinline auto getHeroes() noexcept { return *reinterpret_cast<ManagerTemplate<AIHero>**>(getLeagueModule() + offsets::global::ManagerTemplate_AIHero_); }
	[[nodiscard]] __forceinline auto getMinions() noexcept { return *reinterpret_cast<ManagerTemplate<AIMinionClient>**>(getLeagueModule() + offsets::global::ManagerTemplate_AIMinionClient_); }
	[[nodiscard]] __forceinline auto getRiotWindow() noexcept { return *reinterpret_cast<HWND*>(getLeagueModule() + offsets::global::Riot__g_window); }
	[[nodiscard]] __forceinline auto getChat() noexcept { return *reinterpret_cast<Chat**>(getLeagueModule() + offsets::global::ChatInstance); }
};
