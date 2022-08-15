#pragma once

#include <cstdint>
#include <map>
#include <utility>
#include <vector>

#include "fnv_hash.hpp"
 
class SkinDatabase {
public:
	class skin_info {
	public:
		std::string model_name;
		std::string skin_name;
		std::int32_t skin_id;
	};

	class jungle_mob_skin_info {
	public:
		const char* name;
		std::vector<std::uint32_t> name_hashes;
		std::vector<const char*> skins;
	};

	void load() noexcept;

	std::map<std::uint32_t, std::vector<skin_info>> champions_skins;
	std::vector<std::pair<std::uint32_t, const char*>> wards_skins;
	
	std::vector<const char*> minions_skins{
		"Minion", "Summer Minion",
		"Project Minion", "Snowdown Minion",
		"Draven Minion", "Star Guardian Minion",
		"Arcade Minion", "Snowdown 2 Minion",
		"Odyssey Minion", "Mouse Minion", "Arcane Minion"
	};

	std::vector<const char*> turret_skins{
		"Default Order Turret", "Default Chaos Turret",
		"Snow Order Turret", "Snow Chaos Turret",
		"Twisted Treeline Order Turret", "Twisted Treeline Chaos Turret",
		"URF Order Turret", "URF Chaos Turret",
		"[Broken] Arcade Turret", // crash
		"Temple of Lily and Lotus Turret",
		"Arcane Order Turret", "Arcane Chaos Turret",
		"Butcher's Bridge Order Turret", "Butcher's Bridge Chaos Turret",
		"Howling Abyss Order Turret", "Howling Abyss Chaos Turret"
	};

	std::vector<jungle_mob_skin_info> jungle_mobs_skins{
		{
			"Baron",
			{ FNV("SRU_Baron") },
			{ "Baron", "Snowdown Baron", "Championship Baron", "Lunar Revel Baron", "MSI Baron", "Odyssey Baron", "Championship Birthday Baron", "Ruined King Baron" }
		},
		{
			"Blue",
			{ FNV("SRU_Blue") },
			{ "Blue", "Dark Blue", "Pool Party Blue", "Ruined King Blue" }
		},
		{
			"Red",
			{ FNV("SRU_Red") },
			{ "Red", "Pool Party Red", "Ruined King Red" }
		},
		{
			"Scuttle",
			{ FNV("Sru_Crab") },
			{ "Scuttle", "Halloween Light Scuttle", "Halloween Dark Scuttle", "Ruined King Scuttle" }
		},
		{
			"Krug",
			{ FNV("SRU_Krug"), FNV("SRU_KrugMini"), FNV("SRU_KrugMiniMini") },
			{ "Krug", "Dark Krug" }
		},
		{
			"Razorbeak",
			{ FNV("SRU_Razorbeak"), FNV("SRU_RazorbeakMini") },
			{ "Razorbeak", "Chicken Razorbeak" }
		}
	};
};
