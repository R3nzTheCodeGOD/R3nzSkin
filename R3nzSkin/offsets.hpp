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
		inline std::uintptr_t Player{ 0 };
		inline std::uintptr_t ChampionManager{ 0 };
		inline std::uintptr_t Riot__g_window{ 0 };
		inline std::uintptr_t ManagerTemplate_AIMinionClient_{ 0 };
		inline std::uintptr_t ManagerTemplate_AIHero_{ 0 };
		inline std::uintptr_t GameClient{ 0 };
	};

	namespace AIBaseCommon {
		inline std::uintptr_t CharacterDataStack{ 0 };
		inline std::uintptr_t SkinId{ 0 };
	};

	namespace MaterialRegistry {
		inline std::uintptr_t D3DDevice{ 0 };
#ifdef _RIOT
		inline std::uintptr_t SwapChain{ 0 };
#endif
	};

	namespace functions {
		inline std::uintptr_t Riot__Renderer__MaterialRegistry__GetSingletonPtr{ 0 };
		inline std::uintptr_t translateString_UNSAFE_DONOTUSE{ 0 };
		inline std::uintptr_t CharacterDataStack__Push{ 0 };
		inline std::uintptr_t CharacterDataStack__Update{ 0 };
		inline std::uintptr_t GetGoldRedirectTarget{ 0 };
	};
};
