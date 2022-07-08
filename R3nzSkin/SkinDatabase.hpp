#pragma once

#include <cstdint>
#include <map>
#include <string_view>
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
		std::string_view name;
		std::vector<std::uint32_t> name_hashes;
		std::vector<std::string_view> skins;
	};

	void load() noexcept;

	std::map<std::uint32_t, std::vector<skin_info>> champions_skins;
	std::vector<std::pair<std::uint32_t, std::string_view>> wards_skins;
	std::vector<std::string_view> minions_skins =
	{
		"Minion", "Summer Minion",
		"Project Minion", "Snowdown Minion",
		"Draven Minion", "Star Guardian Minion",
		"Arcade Minion", "Snowdown 2 Minion",
		"Odyssey Minion", "Dvc Minion", "True Shoot Minion"
	};
	std::vector<jungle_mob_skin_info> jungle_mobs_skins =
	{
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
