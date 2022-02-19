#pragma once

#include <Windows.h>
#include <cstdint>
#include <d3d9.h>
#ifdef _RIOT
#include <d3d11.h>
#endif


#include "Offsets.hpp"
#include "SDK/AIBaseCommon.hpp"
#include "SDK/AIHero.hpp"
#include "SDK/AIMinionClient.hpp"
#include "SDK/ChampionManager.hpp"
#include "SDK/GameClient.hpp"
#include "SDK/ManagerTemplate.hpp"

class offset_signature {
public:
	std::vector<std::string> pattern;
	bool sub_base;
	bool read;
	std::int32_t additional;
	std::uint32_t* offset;
};

class Memory {
public:
	void Search(bool gameClient = true) noexcept;
	[[nodiscard]] auto getLeagueModule() const noexcept { return reinterpret_cast<std::uintptr_t>(::GetModuleHandle(nullptr)); }
	[[nodiscard]] auto getRiotWindow() const noexcept { return *reinterpret_cast<HWND*>(getLeagueModule() + offsets::global::Riot__g_window); }

	GameClient* client;
	AIBaseCommon* localPlayer;
	ManagerTemplate<AIHero>* heroList;
	ManagerTemplate<AIMinionClient>* minionList;
	ChampionManager* championManager;
	const char*(__cdecl* translateString)(const char*);
	std::uintptr_t materialRegistry;
	IDirect3DDevice9* d3dDevice;
#ifdef _RIOT
	IDXGISwapChain* swapChain;
#endif
private:
	void update(bool gameClient = true) noexcept;

	std::vector<offset_signature> gameClientSig
	{
		{
			{
				"A1 ? ? ? ? 56 83 78 08 00",
				"A1 ? ? ? ? 68 ? ? ? ? 8B 70 08",
				"A1 ? ? ? ? 83 78 08 02 0F 85 ? ? ? ?"
			},
			true, true, 0,
			&offsets::global::GameClient
		}
	};

	std::vector<offset_signature> sigs
	{
		{
			{
				"8B 3D ? ? ? ? 3B F7 75 09",
				"A1 ? ? ? ? 8B 54 24 28 85 C0",
				"8B 0D ? ? ? ? 85 C9 0F 84 ? ? ? ? 83 7E 10 00"
			},
			true, true, 0,
			& offsets::global::Player
		},
		{
			{
				"A1 ? ? ? ? 8B 54 24 14 53",
				"8B 0D ? ? ? ? 50 8D 44 24 18 C7 44 24 ? ? ? ? ?",
				"8B 0D ? ? ? ? 6A 00 E8 ? ? ? ? 8B CE"
			},
			true, true, 0,
			&offsets::global::ManagerTemplate_AIHero_
		},
		{
			{
				"89 1D ? ? ? ? 57 8D 4B 08",
				"8B 3D ? ? ? ? 85 FF 74 2B 8B 4F 1C"
			},
			true, true, 0,
			&offsets::global::ChampionManager
		},
		{
			{
				"A1 ? ? ? ? 53 55 8B 6C 24 1C",
				"8B 35 ? ? ? ? 8B 56 04",
				"8B 35 ? ? ? ? 8B 49 08 E8 ? ? ? ? F3 0F 10 0D ? ? ? ?"
			},
			true, true, 0,
			&offsets::global::ManagerTemplate_AIMinionClient_
		},
		{
			{
				"3B 05 ? ? ? ? 75 72",
				"A3 ? ? ? ? 6A 64 6A 00",
				"FF 35 ? ? ? ? E8 ? ? ? ? 83 C4 0C 80 7E 2C 00"
			},
			true, true, 0,
			&offsets::global::Riot__g_window
		},
		{
			{
				"8D 8E ? ? ? ? FF 74 24 4C"
			},
			false, true, 0,
			&offsets::AIBaseCommon::CharacterDataStack
		},
		{
			{
				"80 BE ? ? ? ? ? 75 4D 0F 31"
			},
			false, true, 0,
			&offsets::AIBaseCommon::SkinId
		},
		{
			{
				"8B B7 ? ? ? ? FF 70 28",
				"8B 8F ? ? ? ? 88 A7 ? ? ? ?",
				"8B 8E ? ? ? ? D1 E8",
				"8B 86 ? ? ? ? 89 4C 24 08",
				"8B 8E ? ? ? ? C1 E8 02",
			},
			false, true, 0,
			&offsets::MaterialRegistry::D3DDevice
		},
	#ifdef _RIOT
		{
			{
				"8B 95 ? ? ? ? 0F 10 44 24 ?",
				"8D 9F ? ? ? ? 0F 84 ? ? ? ?",
				"8B 8E ? ? ? ? 52 57"
			},
			false, true, 0,
			&offsets::MaterialRegistry::SwapChain
		},
	#endif
		{
			{
				"83 EC 50 53 55 56 57 8B F9 8B 47 04"
			},
			true, false, 0,
			&offsets::functions::CharacterDataStack__Push },
		{
			{
				"E8 ? ? ? ? 5F 5E 5B 83 C4 44",
				"E8 ? ? ? ? 8B 4E 7C 5E"
			},
			true, false, 0,
			&offsets::functions::CharacterDataStack__Update
		},
		{
			{
				"E8 ? ? ? ? FF 75 38",
				"E8 ? ? ? ? FF 73 58",
			},
			true, false, 0,
			&offsets::functions::Riot__Renderer__MaterialRegistry__GetSingletonPtr
		},
		{
			{
				"E8 ? ? ? ? FF 75 FC 50",
				"E8 ? ? ? ? 8B 0D ? ? ? ? 83 C4 04 8B F0 6A 0B",
				"E8 ? ? ? ? 83 C4 04 8D 4E 0C 8B D0"
			},
			true, false, 0,
			&offsets::functions::translateString_UNSAFE_DONOTUSE
		},
		{
			{
				"E8 ? ? ? ? 39 44 24 1C 5F",
				"E8 ? ? ? ? 85 C0 74 3A"
			},
			true, false, 0,
			&offsets::functions::GetGoldRedirectTarget
		}
	};
};
