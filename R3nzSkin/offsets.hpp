#pragma once

#include <cstdint>

namespace offsets {
	namespace GameObject {
		namespace VTable {
			enum {
				IsLaneMinion = 0xDA,
				IsEliteMinion = IsLaneMinion + 0x1,
				IsEpicMinion = IsEliteMinion + 0x1,
				IsMinion = IsEpicMinion + 0x4,
				IsJungle = IsMinion + 0x1
			};
		};
		enum {
			Team = 0x34,
			Name = 0x54
		};
	};

	namespace global {
		inline std::uint32_t Player{ 0 };
		inline std::uint32_t ChampionManager{ 0 };
		inline std::uint32_t Riot__g_window{ 0 };
		inline std::uint32_t ManagerTemplate_AIMinionClient_{ 0 };
		inline std::uint32_t ManagerTemplate_AIHero_{ 0 };
		inline std::uint32_t ManagerTemplate_AITurret_{ 0 };
		inline std::uint32_t GameClient{ 0 };
		inline std::uint32_t retSpoofGadget{ 0 };
	};

	namespace AIBaseCommon {
		inline std::uint32_t CharacterDataStack{ 0 };
		inline std::uint32_t SkinId{ 0 };
	};

	namespace MaterialRegistry {
		inline std::uint32_t D3DDevice{ 0 };
		inline std::uint32_t SwapChain{ 0 };
	};

	namespace functions {
		inline std::uint32_t Riot__Renderer__MaterialRegistry__GetSingletonPtr{ 0 };
		inline std::uint32_t translateString_UNSAFE_DONOTUSE{ 0 };
		inline std::uint32_t CharacterDataStack__Push{ 0 };
		inline std::uint32_t CharacterDataStack__Update{ 0 };
		inline std::uint32_t GetGoldRedirectTarget{ 0 };
	};
};
