#include <exception>
#include <fstream>
#include <string>

#include "Json/json.hpp"

#include "CheatManager.hpp"
#include "Memory.hpp"
#include "Utils.hpp"

void Config::save() noexcept
{
	try {
		const auto player{ cheatManager.memory->localPlayer };
		auto out{ std::ofstream(L"R3nzSkin.json") };

		if (player)
			config_json[std::string(player->get_character_data_stack()->base_skin.model.str) + ".current_combo_skin_index"] = config.current_combo_skin_index;

		config_json["menuKey"] = config.menuKey.toString();
		config_json["nextSkinKey"] = config.nextSkinKey.toString();
		config_json["previousSkinKey"] = config.previousSkinKey.toString();
		config_json["heroName"] = config.heroName;
		config_json["raibowText"] = config.rainbowText;
		config_json["quickSkinChange"] = config.quickSkinChange;
		config_json["current_combo_ward_index"] = config.current_combo_ward_index;
		config_json["current_ward_skin_index"] = config.current_ward_skin_index;
		config_json["current_minion_skin_index"] = config.current_minion_skin_index;

		for (const auto& it : config.current_combo_ally_skin_index)
			config_json["current_combo_ally_skin_index"][std::to_string(it.first)] = it.second;

		for (const auto& it : config.current_combo_enemy_skin_index)
			config_json["current_combo_enemy_skin_index"][std::to_string(it.first)] = it.second;

		for (const auto& it : config.current_combo_jungle_mob_skin_index)
			config_json["current_combo_jungle_mob_skin_index"][std::to_string(it.first)] = it.second;

		out << config_json.dump();
		out.close();
	} catch (std::exception& error) {
		MessageBoxA(nullptr, error.what(), "R3nzSkin", MB_OK | MB_ICONWARNING);
	}
}

void Config::load() noexcept
{
	try {
		const auto player{ cheatManager.memory->localPlayer };
		auto out{ std::ifstream(L"R3nzSkin.json") };

		if (!out.good())
			return;

		config_json = json::parse(out);

		if (player)
			config.current_combo_skin_index = config_json.value(std::string(player->get_character_data_stack()->base_skin.model.str) + ".current_combo_skin_index", 0);

		config.menuKey = KeyBind(config_json.value("menuKey", "INSERT").c_str());
		config.nextSkinKey = KeyBind(config_json.value("nextSkinKey", "PAGE_UP").c_str());
		config.previousSkinKey = KeyBind(config_json.value("previousSkinKey", "PAGE_DOWN").c_str());
		config.heroName = config_json.value("heroName", true);
		config.rainbowText = config_json.value("raibowText", false);
		config.quickSkinChange = config_json.value("quickSkinChange", false);
		config.current_combo_ward_index = config_json.value("current_combo_ward_index", 0);
		config.current_ward_skin_index = config_json.value("current_ward_skin_index", -1);
		config.current_minion_skin_index = config_json.value("current_minion_skin_index", -1);

		const auto ally_skins{ config_json.find("current_combo_ally_skin_index") };
		if (ally_skins != config_json.end())
			for (const auto& it : ally_skins.value().items())
				config.current_combo_ally_skin_index[std::stoul(it.key())] = it.value().get<std::int32_t>();

		const auto enemy_skins{ config_json.find("current_combo_enemy_skin_index") };
		if (enemy_skins != config_json.end())
			for (const auto& it : enemy_skins.value().items())
				config.current_combo_enemy_skin_index[std::stoul(it.key())] = it.value().get<std::int32_t>();

		const auto jungle_mobs_skins{ config_json.find("current_combo_jungle_mob_skin_index") };
		if (jungle_mobs_skins != config_json.end())
			for (const auto& it : jungle_mobs_skins.value().items())
				config.current_combo_jungle_mob_skin_index[std::stoul(it.key())] = it.value().get<std::int32_t>();

		out.close();
	} catch (std::exception& error) {
		MessageBoxA(nullptr, error.what(), "R3nzSkin", MB_OK | MB_ICONWARNING);
	}
}

void Config::reset() noexcept
{
	config.menuKey = KeyBind(KeyBind::INSERT);
	config.nextSkinKey = KeyBind(KeyBind::PAGE_UP);
	config.previousSkinKey = KeyBind(KeyBind::PAGE_DOWN);
	config.heroName = true;
	config.rainbowText = true;
	config.quickSkinChange = false;
	config.current_combo_skin_index = 0;
	config.current_combo_ward_index = 0;
	config.current_combo_minion_index = 0;
	config.current_minion_skin_index = -1;
	config.current_ward_skin_index = -1;
	config.current_combo_ally_skin_index.clear();
	config.current_combo_enemy_skin_index.clear();
	config.current_combo_jungle_mob_skin_index.clear();
}
