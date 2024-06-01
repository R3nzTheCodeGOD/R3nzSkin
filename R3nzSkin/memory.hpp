#pragma once

#include <Windows.h>
#include <cstdint>
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
	bool relative;
	std::int32_t additional;
	std::uint64_t* offset;
};

class Memory {
public:
	void Search(bool gameClient = true);

	std::uintptr_t base;
	HWND window;

	GameClient* client;
	AIBaseCommon* localPlayer;
	ManagerTemplate<AIHero>* heroList;
	ManagerTemplate<AIMinionClient>* minionList;
	ManagerTemplate<AITurret>* turretList;
	ChampionManager* championManager;
	
	std::uintptr_t materialRegistry;
	IDXGISwapChain* swapChain;

	using translateString_t = const char* (__fastcall*)(const char*);

	translateString_t translateString;
private:
	void update(bool gameClient = true) noexcept;

	std::vector<offset_signature> gameClientSig
	{
		{
			{
				"48 8B 05 ? ? ? ? 4C 8B FA 83 78 0C 02"
			}, true, false, true, 0, &offsets::global::GameClient
		}
	};

	std::vector<offset_signature> sigs
	{
		{
			{
				"48 8B 3D ? ? ? ? 48 3B CF"
			}, true, false, true, 0, &offsets::global::Player
		},
		{
			{
				"48 8B 05 ? ? ? ? 48 8B 78 08 8B 40 10 4C 8D 34 C7 49 3B FE"
			}, true, false, true, 0, &offsets::global::ManagerTemplate_AIHero_
		},
		{
			{
				"48 8B 0D ? ? ? ? 48 69 D0 ? ? 00 00 48 8B 05"
			}, true, false, true, 0, &offsets::global::ChampionManager
		},
		{
			{
				"48 8B 0D ? ? ? ? E8 ? ? ? ? E8 ? ? ? ? E8 ? ? ? ? 48 8B C8 48 8B 10"
			}, true, false, true, 0, &offsets::global::ManagerTemplate_AIMinionClient_
		},
		{
			{
				"48 8B 1D ? ? ? ? 48 8B 5B 28 48 85 DB"
			}, true, false, true, 0, &offsets::global::ManagerTemplate_AITurret_
		},
		{
			{
				"48 8B 0D ? ? ? ? FF 15 ? ? ? ? 48 8B 05 ? ? ? ?"
			}, true, false, true, 0, &offsets::global::Riot__g_window
		},
		{
			{
				"48 8D 8D ? ? 00 00 44 8B 8C 24 ? ? 00 00"
			}, false, true, false, 0, &offsets::AIBaseCommon::CharacterDataStack
		},
		{
			{
				"40 38 BB ? ? 00 00 0F 85 ? ? 00 00 66 C7 83 ? ? ? ? ? ? 0F 31 48 C1 E2 20 4C 8D 83 ? ? 00 00 48 0B C2 44 8B CF 48 89 44 24 38 8B D7"
			}, false, true, false, 0, &offsets::AIBaseCommon::SkinId
		},
		{
			{
				"48 8D BB ? ? ? ? C6 83 ? ? ? ? ? 0F 84"
			}, false, true, false, 0, &offsets::MaterialRegistry::SwapChain
		},
		{
			{
				"E8 ? ? ? ? 48 8D 8D ? ? 00 00 E8 ? ? ? ? 48 85 C0 74 ? 48 85 ED"
			}, true, false, false, 0, &offsets::functions::CharacterDataStack__Push
		},
		{
			{
				"88 54 24 10 53 55 56 57 41 54 41 55 41 56 41"
			}, true, false, false, 0, &offsets::functions::CharacterDataStack__Update
		},
		{
			{
				"E8 ? ? ? ? 8B 57 44"
			}, true, false, false, 0, &offsets::functions::Riot__Renderer__MaterialRegistry__GetSingletonPtr
		},
		{
			{
				"E8 ? ? ? ? 0F 57 DB 4C 8B C0 F3 0F 5A DE"
			}, true, false, false, 0, &offsets::functions::translateString_UNSAFE_DONOTUSE
		},
		{
			{
				"E8 ? ? ? ? 4C 3B F8 0F 94 C0"
			}, true, false, false, 0, &offsets::functions::GetGoldRedirectTarget
		}
	};
};
