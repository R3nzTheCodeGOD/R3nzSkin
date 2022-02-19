#include <algorithm>
#include <cstdint>
#include <map>
#include <ranges>
#include <utility>

#include "CheatManager.hpp"
#include "SkinDatabase.hpp"
#include "fnv_hash.hpp"

void SkinDatabase::load() noexcept
{
	const auto translateString{ cheatManager.memory->translateString };

	for (const auto& champion : cheatManager.memory->championManager->champions) {
		std::vector<std::int32_t> skins_ids;
		
		for (const auto& skin : champion->skins)
			skins_ids.push_back(skin.skin_id);
		
		std::ranges::sort(skins_ids);

		std::map<std::string, std::int32_t> temp_skin_list;
		for (const auto& i : skins_ids) {
			const auto skin_display_name{ std::string("game_character_skin_displayname_") + champion->champion_name.str + "_" + std::to_string(i) };

			auto skin_display_name_translated{ i > 0 ? std::string(translateString(skin_display_name.c_str())) : std::string(champion->champion_name.str) };
			const auto it{ temp_skin_list.find(skin_display_name_translated) };

			if (it == temp_skin_list.end())
				temp_skin_list[skin_display_name_translated] = 1;
			else {
				skin_display_name_translated.append(" Chroma " + std::to_string(it->second));
				it->second = it->second + 1;
			}

			const auto champ_name{ fnv::hash_runtime(champion->champion_name.str) };
			this->champions_skins[champ_name].push_back(skin_info{ std::string(champion->champion_name.str), skin_display_name_translated, i });

			if (i == 7 && champ_name == FNV("Lux")) {
				this->champions_skins[champ_name].push_back(skin_info{ "LuxAir", "Elementalist Air Lux", i });
				this->champions_skins[champ_name].push_back(skin_info{ "LuxDark", "Elementalist Dark Lux", i });
				this->champions_skins[champ_name].push_back(skin_info{ "LuxFire", "Elementalist Fire Lux", i });
				this->champions_skins[champ_name].push_back(skin_info{ "LuxIce", "Elementalist Ice Lux", i });
				this->champions_skins[champ_name].push_back(skin_info{ "LuxMagma", "Elementalist Magma Lux", i });
				this->champions_skins[champ_name].push_back(skin_info{ "LuxMystic", "Elementalist Mystic Lux", i });
				this->champions_skins[champ_name].push_back(skin_info{ "LuxNature", "Elementalist Nature Lux", i });
				this->champions_skins[champ_name].push_back(skin_info{ "LuxStorm", "Elementalist Storm Lux", i });
				this->champions_skins[champ_name].push_back(skin_info{ "LuxWater", "Elementalist Water Lux", i });
			} else if (i == 6 && champ_name == FNV("Sona")) {
				this->champions_skins[champ_name].push_back(skin_info{ "SonaDJGenre02", "DJ Sona 2", i });
				this->champions_skins[champ_name].push_back(skin_info{ "SonaDJGenre03", "DJ Sona 3", i });
			}
		}
	}

	for (auto ward_skin_id{ 1u };; ++ward_skin_id) {
		const auto ward_display_name{ "game_character_skin_displayname_SightWard_" + std::to_string(ward_skin_id) };
		const auto ward_display_name_translated{ std::string(translateString(ward_display_name.c_str())) };
		
		if (ward_display_name == ward_display_name_translated)
			break;

		this->wards_skins.push_back({ ward_skin_id, ward_display_name_translated });
	}
}
