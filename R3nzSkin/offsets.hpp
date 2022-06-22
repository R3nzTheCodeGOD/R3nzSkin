#pragma once

#include <cstdint>

namespace offsets {
	namespace GameObject {
		enum {
			Team = 0x34,
			Name = 0x54,
			Position = 0x1DC,
			Visible = 0x274
		};
	};

	namespace global {
		inline std::uint32_t Player{ 0 };
		inline std::uint32_t ChampionManager{ 0 };
		inline std::uint32_t Riot__g_window{ 0 };
		inline std::uint32_t ManagerTemplate_AIMinionClient_{ 0 };
		inline std::uint32_t ManagerTemplate_AIHero_{ 0 };
		inline std::uint32_t GameClient{ 0 };
		inline std::uint32_t GameTime{ 0 };
	};

	namespace SpellBook {
		inline std::uint32_t SpellBook{ 0 };
		inline std::uint32_t SpellSlot{ 0 };
	};

	namespace AIBaseCommon {
		inline std::uint32_t CharacterDataStack{ 0 };
		inline std::uint32_t SkinId{ 0 };
	};

	namespace MaterialRegistry {
		inline std::uint32_t D3DDevice{ 0 };
		inline std::uint32_t SwapChain{ 0 };
	};

	namespace AIMinionClient {
		inline std::uint32_t IsLaneMinion{ 0 };
	};

	namespace functions {
		inline std::uint32_t Riot__Renderer__MaterialRegistry__GetSingletonPtr{ 0 };
		inline std::uint32_t translateString_UNSAFE_DONOTUSE{ 0 };
		inline std::uint32_t CharacterDataStack__Push{ 0 };
		inline std::uint32_t CharacterDataStack__Update{ 0 };
		inline std::uint32_t GetGoldRedirectTarget{ 0 };
		inline std::uint32_t WorldToScreen{ 0 };
		inline std::uint32_t IsAlive{ 0 };
	};
};
