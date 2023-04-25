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
		const char* model_name;
		std::string skin_name;
		std::int32_t skin_id;
	};

	class jungle_mob_skin_info {
	public:
		const char* name;
		std::vector<std::uint64_t> name_hashes;
		std::vector<const char*> skins;
	};

	class specialSkin {
	public:
		std::uint64_t champHash;
		std::int32_t skinIdStart;
		std::int32_t skinIdEnd;
		std::vector<const char*> gears;
	};

	void load() noexcept;

	std::map<std::uint64_t, std::vector<skin_info>> champions_skins;
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
		"Arcade Turret",
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

	std::vector<specialSkin> specialSkins{
		{ FNV("Katarina"), 29, 36, { "Dagger 1", "Dagger 2", "Dagger 3", "Dagger 4", "Dagger 5", "Dagger 6" }},
		{ FNV("Renekton"), 26, 32, { "Head off", "Head on", "Fins", "Ultimate" } },
		{ FNV("MissFortune"), 16, 16, { "Scarlet fair", "Zero hour", "Royal arms", "Starswarm" } },
		{ FNV("Ezreal"), 5, 5, { "Level 1", "Level 2", "Level 3" } }
	};
};
