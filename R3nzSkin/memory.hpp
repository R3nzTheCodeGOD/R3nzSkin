#pragma once

#include <Windows.h>
#include <cstdint>
#include <d3d9.h>
#include <d3d11.h>

#include "Offsets.hpp"
#include "SDK/AIBaseCommon.hpp"
#include "SDK/AIHero.hpp"
#include "SDK/AITurret.hpp"
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
	[[nodiscard]] inline auto getLeagueModule() const noexcept { return reinterpret_cast<std::uintptr_t>(::GetModuleHandle(nullptr)); }
	[[nodiscard]] inline auto getRiotWindow() const noexcept { return *reinterpret_cast<HWND*>(this->getLeagueModule() + offsets::global::Riot__g_window); }

	GameClient* client;
	AIBaseCommon* localPlayer;
	ManagerTemplate<AIHero>* heroList;
	ManagerTemplate<AIMinionClient>* minionList;
	ManagerTemplate<AITurret>* turretList;
	ChampionManager* championManager;
	std::uintptr_t materialRegistry;
	IDirect3DDevice9* d3dDevice;
	IDXGISwapChain* swapChain;

	using FnTranlateString = const char*(__cdecl*)(const char*);
	
	FnTranlateString translateString;
private:
	void update(bool gameClient = true) noexcept;

	std::vector<offset_signature> gameClientSig
	{
		{
			{
				"A1 ? ? ? ? 68 ? ? ? ? 8B 70 08 E8 ? ? ? ?",
				"A1 ? ? ? ? 56 83 78 08 00 75 ?"
			}, true, true, 0, &offsets::global::GameClient
		}
	};

	std::vector<offset_signature> sigs
	{
		{
			{
				"8B 3D ? ? ? ? 3B F7 75 09 5F 5E",
				"A1 ? ? ? ? 8B 54 24 28 85 C0 0F 84 ? ? ? ?"
			}, true, true, 0, &offsets::global::Player
		},
		{
			{
				"A1 ? ? ? ? 8B 54 24 14 53 55",
				"8B 0D ? ? ? ? E8 ? ? ? ? 85 C0 74 24 05 ? ? ? ? C2 04 00"
			}, true, true, 0, &offsets::global::ManagerTemplate_AIHero_
		},
		{
			{
				"89 1D ? ? ? ? 57 8D 4B 08 89 5C 24 ? C7 03 ? ? ? ? E8 ? ? ? ?",
				"8B 0D ? ? ? ? 83 C1 18 89 14 24 8B 41 04 3B 41 08"
			}, true, true, 0, &offsets::global::ChampionManager
		},
		{
			{
				"8B 35 ? ? ? ? 8B 56 04 8D 4E 04 8B 41 04 8D 04 82",
				"A1 ? ? ? ? 55 57 8B 78 04 8B 40 08 8D 2C 87 3B FD"
			}, true, true, 0, &offsets::global::ManagerTemplate_AIMinionClient_
		},
		{
			{
				"8B 35 ? ? ? ? 8B 76 18 85 F6 74 ?",
				"89 35 ? ? ? ? 57 68 ? ? ? ? C7 46 04 00 00 00 00 C7 46 08 00 00 00 00"
			}, true, true, 0, &offsets::global::ManagerTemplate_AITurret_
		},
		{
			{
				"A3 ? ? ? ? 6A 64 6A 00",
				"8B 35 ? ? ? ? FF 15 ? ? ? ? 3B C6 75 ? 8B 0D ? ? ? ? 85 C9 74 ? 8B 01"
			}, true, true, 0, &offsets::global::Riot__g_window
		},
		{
			{
				"8D 8E ? ? ? ? FF 74 24 4C",
				"8D 8E ? ? ? ? FF 74 24 ? FF 74 24 ? E8 ? ? ? ? 8B 74 ? ? 85 F6 74 ? 8B C7 F0 0F C1 46 04"
			}, false, true, 0, &offsets::AIBaseCommon::CharacterDataStack
		},
		{
			{
				"80 BE ? ? 00 00 00 75 ? 0F 31 33 C9 66 C7 86 ? ? ? ? 01 00 89 44 24 ? 33 DB 89 54 24 ? 8A 44 0C ? 41 88 84 33 ? ? 00 00",
			}, false, true, 0, &offsets::AIBaseCommon::SkinId
		},
		{
			{
				"8B B7 ? ? ? ? FF 70 28 8B 0E 56",
				"8B 8F ? ? ? ? 88 A7 ? ? ? ? 0F B6 C4 8B 11"
			}, false, true, 0, &offsets::MaterialRegistry::D3DDevice
		},
		{
			{
				"8D 9F ? ? ? ? 0F 84 ? ? ? ? 8B 0B 6A 00 6A 00 51  8B 01 FF 50 ? 85 C0",
				"8B 95 ? ? ? ? 0F 10 44 24 ? 56 8B 02 66 0F 73 D8 ?"
			}, false, true, 0, &offsets::MaterialRegistry::SwapChain
		},
		{
			{
				"0F B6 97 ? ? ? ? 88 4C 24 ? 33 C9 85 D2 74 ? 0F 1F 44 00 00"
			}, false, true, -1, &offsets::AIMinionClient::IsLaneMinion
		},
		{
			{
				"83 EC 74 53 55 56 57 8B F9 8B 47 04 39 07"
			}, true, false, 0, &offsets::functions::CharacterDataStack__Push },
		{
			{
				"83 EC 1C 56 57 8D 44 24 ? 8B F1 50 8D 44 24 ? 50 8D 44 24 ? 50 8D 44 24 ? 50 8D 44 24 ? 50 8D 44 24 ? 50",
				"E8 ? ? ? ? 8B 8E ? ? ? ? 5E 8B 01 FF 60 ?"
			}, true, false, 0, &offsets::functions::CharacterDataStack__Update
		},
		{
			{
				"E8 ? ? ? ? FF 73 58 8B C8 8B 10 FF 52 ?",
				"E8 ? ? ? ? 6A 00 6A 02 6A 00 8B 08 57 8B 51 ? 8B C8 FF D2 89 45 ?",
			}, true, false, 0, &offsets::functions::Riot__Renderer__MaterialRegistry__GetSingletonPtr
		},
		{
			{
				"E8 ? ? ? ? 8B 0D ? ? ? ? 83 C4 04 8B F0 6A 0B E8 ? ? ? ? 56 50 55 57",
				"E8 ? ? ? ? 83 C4 04 8D 4E 0C 8B D0 80 38 00 5E 74 0C"
			}, true, false, 0, &offsets::functions::translateString_UNSAFE_DONOTUSE
		},
		{
			{
				"E8 ? ? ? ? 3B F8 5F 5E 5D 0F 94 C0 5B",
				"E8 ? ? ? ? 85 C0 74 09 8B CE"
			}, true, false, 0, &offsets::functions::GetGoldRedirectTarget
		}
	};
};
