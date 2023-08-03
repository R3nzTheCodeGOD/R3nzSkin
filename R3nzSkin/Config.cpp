#include <fstream>
#include <string>
#include <system_error>

#include <Windows.h>
#include <ShlObj.h>

#include "Json/json.hpp"

#include "CheatManager.hpp"
#include "Memory.hpp"
#include "Utils.hpp"

void Config::init() noexcept
{
	if (PWSTR pathToDocuments; SUCCEEDED(SHGetKnownFolderPath(FOLDERID_Documents, 0, nullptr, &pathToDocuments))) {
		this->path = pathToDocuments;
		CoTaskMemFree(pathToDocuments);
	}

	this->path /= "R3nzSkin";
}

void Config::save() noexcept
{
	const auto player{ cheatManager.memory->localPlayer };
	std::error_code ec;
	std::filesystem::create_directory(this->path, ec);
	auto out{ std::ofstream(this->path / u8"R3nzSkin64")};

	if (!out.good())
		return;

	if (player)
		config_json[std::string(player->get_character_data_stack()->base_skin.model.str) + ".current_combo_skin_index"] = this->current_combo_skin_index;

	config_json["menuKey"] = this->menuKey.toString();
	config_json["nextSkinKey"] = this->nextSkinKey.toString();
	config_json["previousSkinKey"] = this->previousSkinKey.toString();
	config_json["heroName"] = this->heroName;
	config_json["raibowText"] = this->rainbowText;
	config_json["quickSkinChange"] = this->quickSkinChange;
	config_json["fontScale"] = this->fontScale;
	config_json["current_combo_ward_index"] = this->current_combo_ward_index;
	config_json["current_ward_skin_index"] = this->current_ward_skin_index;
	config_json["current_minion_skin_index"] = this->current_minion_skin_index;

	for (const auto& [fst, snd] : this->current_combo_ally_skin_index)
		config_json["current_combo_ally_skin_index"][std::to_string(fst)] = snd;

	for (const auto& [fst, snd] : this->current_combo_enemy_skin_index)
		config_json["current_combo_enemy_skin_index"][std::to_string(fst)] = snd;

	for (const auto& [fst, snd] : this->current_combo_jungle_mob_skin_index)
		config_json["current_combo_jungle_mob_skin_index"][std::to_string(fst)] = snd;

	out << config_json.dump();
	out.close();
}

void Config::load() noexcept
{
	const auto player{ cheatManager.memory->localPlayer };
	auto in{ std::ifstream(this->path / u8"R3nzSkin64") };

	if (!in.good())
		return;

	if (json j{ json::parse(in, nullptr, false, true) }; j.is_discarded())
		return;
	else
		config_json = j;

	if (player)
		this->current_combo_skin_index = config_json.value(std::string(player->get_character_data_stack()->base_skin.model.str) + ".current_combo_skin_index", 0);

	this->menuKey = KeyBind(config_json.value("menuKey", "INSERT").c_str());
	this->nextSkinKey = KeyBind(config_json.value("nextSkinKey", "PAGE_UP").c_str());
	this->previousSkinKey = KeyBind(config_json.value("previousSkinKey", "PAGE_DOWN").c_str());
	this->heroName = config_json.value("heroName", true);
	this->rainbowText = config_json.value("raibowText", false);
	this->quickSkinChange = config_json.value("quickSkinChange", false);
	this->fontScale = config_json.value("fontScale", 1.0f);
	this->current_combo_ward_index = config_json.value("current_combo_ward_index", 0);
	this->current_ward_skin_index = config_json.value("current_ward_skin_index", -1);
	this->current_minion_skin_index = config_json.value("current_minion_skin_index", -1);

	const auto ally_skins{ config_json.find("current_combo_ally_skin_index") };
	if (ally_skins != config_json.end())
		for (const auto& it : ally_skins.value().items())
			this->current_combo_ally_skin_index[std::stoull(it.key())] = it.value().get<std::int32_t>();

	const auto enemy_skins{ config_json.find("current_combo_enemy_skin_index") };
	if (enemy_skins != config_json.end())
		for (const auto& it : enemy_skins.value().items())
			this->current_combo_enemy_skin_index[std::stoull(it.key())] = it.value().get<std::int32_t>();

	const auto jungle_mobs_skins{ config_json.find("current_combo_jungle_mob_skin_index") };
	if (jungle_mobs_skins != config_json.end())
		for (const auto& it : jungle_mobs_skins.value().items())
			this->current_combo_jungle_mob_skin_index[std::stoull(it.key())] = it.value().get<std::int32_t>();

	in.close();
}

void Config::reset() noexcept
{
	this->menuKey = KeyBind(KeyBind::INSERT);
	this->nextSkinKey = KeyBind(KeyBind::PAGE_UP);
	this->previousSkinKey = KeyBind(KeyBind::PAGE_DOWN);
	this->heroName = true;
	this->rainbowText = true;
	this->quickSkinChange = false;
	this->fontScale = 1.0f;
	this->current_combo_skin_index = 0;
	this->current_combo_ward_index = 0;
	this->current_combo_minion_index = 0;
	this->current_minion_skin_index = -1;
	this->current_ward_skin_index = -1;
	this->current_combo_order_turret_index = 0;
	this->current_combo_chaos_turret_index = 0;
	this->current_combo_ally_skin_index.clear();
	this->current_combo_enemy_skin_index.clear();
	this->current_combo_jungle_mob_skin_index.clear();
}
