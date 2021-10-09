#include <Windows.h>
#include <fstream>
#include <string>

#include "Json/json.hpp"

#include "Config.hpp"
#include "GameClasses.hpp"
#include "Memory.hpp"
#include "Offsets.hpp"

void Config::save() noexcept
{
	const auto player{ Memory::getLocalPlayer() };
	auto out{ std::ofstream(L"R3nzSkin.json") };

	if (player)
		config_json[std::string(player->get_character_data_stack()->base_skin.model.str) + ".current_combo_skin_index"] = config.current_combo_skin_index;

	config_json["heroName"] = config.heroName;
	config_json["raibowText"] = config.rainbowText;
	config_json["current_combo_ward_index"] = config.current_combo_ward_index;
	config_json["current_ward_skin_index"] = config.current_ward_skin_index;

	for (const auto& it : config.current_combo_ally_skin_index)
		config_json["current_combo_ally_skin_index"][std::to_string(it.first)] = it.second;

	for (const auto& it : config.current_combo_enemy_skin_index)
		config_json["current_combo_enemy_skin_index"][std::to_string(it.first)] = it.second;

	out << config_json.dump();
	out.close();
}

void Config::load() noexcept
{
	const auto player{ Memory::getLocalPlayer() };
	auto out{ std::ifstream(L"R3nzSkin.json") };

	if (!out.good())
		return;

	config_json = json::parse(out);

	if (player)
		config.current_combo_skin_index = config_json.value(std::string(player->get_character_data_stack()->base_skin.model.str) + ".current_combo_skin_index", 0);
	
	config.heroName = config_json.value("heroName", true);
	config.rainbowText = config_json.value("raibowText", false);
	config.current_combo_ward_index = config_json.value("current_combo_ward_index", 0);
	config.current_ward_skin_index = config_json.value("current_ward_skin_index", -1);

	const auto ally_skins{ config_json.find("current_combo_ally_skin_index") };
	if (ally_skins != config_json.end())
		for (const auto& it : ally_skins.value().items())
			config.current_combo_ally_skin_index[std::stoul(it.key())] = it.value().get<int32_t>();

	const auto enemy_skins{ config_json.find("current_combo_enemy_skin_index") };
	if (enemy_skins != config_json.end())
		for (const auto& it : enemy_skins.value().items())
			config.current_combo_enemy_skin_index[std::stoul(it.key())] = it.value().get<int32_t>();

	out.close();
}

void Config::reset() noexcept
{
	config.heroName = true;
	config.rainbowText = true;
	config.current_combo_skin_index = 0;
	config.current_combo_ward_index = 0;
	config.current_ward_skin_index = -1;
	config.current_combo_ally_skin_index.clear();
	config.current_combo_enemy_skin_index.clear();
}
