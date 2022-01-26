#pragma once

#include <cstdint>

namespace offsets {
	namespace GameObject {
		enum {
			ID = 0x20,
			Team = 0x4C,
			Name = 0x6C
		};
	};

	namespace global {
		inline std::uint32_t Player{ 0 };
		inline std::uint32_t ChampionManager{ 0 };
		inline std::uint32_t Riot__g_window{ 0 };
		inline std::uint32_t ManagerTemplate_AIMinionClient_{ 0 };
		inline std::uint32_t ManagerTemplate_AIHero_{ 0 };
		inline std::uint32_t GameClient{ 0 };
	};

	namespace AIBaseCommon {
		inline std::uint32_t CharacterDataStack{ 0 };
		inline std::uint32_t SkinId{ 0 };
	};

	namespace MaterialRegistry {
		inline std::uint32_t D3DDevice{ 0 };
#ifdef _RIOT
		inline std::uint32_t SwapChain{ 0 };
#endif
	};

	namespace functions {
		inline std::uint32_t Riot__Renderer__MaterialRegistry__GetSingletonPtr{ 0 };
		inline std::uint32_t translateString_UNSAFE_DONOTUSE{ 0 };
		inline std::uint32_t CharacterDataStack__Push{ 0 };
		inline std::uint32_t CharacterDataStack__Update{ 0 };
		inline std::uint32_t GetGoldRedirectTarget{ 0 };
	};
};

// oIsAlive	 = "56 8B F1 8B 06 8B 80 ? ? ? ? FF D0 84 C0 74 19"
// oIsHero	 = "E8 ? ? ? ? 83 C4 04 84 C0 74 2B 57"
// oRenderer = "8B 15 ? ? ? ? 83 EC 08 F3"
