#pragma once

#include <cstdint>
#include <filesystem>
#include <map>

#include "json/json.hpp"

#include "Utils.hpp"

using json = nlohmann::json;

class Config {
public:
	void init() noexcept;
	void save() noexcept;
	void load() noexcept;
	void reset() noexcept;

	KeyBind menuKey{ KeyBind(KeyBind::INSERT) };
	KeyBind nextSkinKey{ KeyBind(KeyBind::PAGE_UP) };
	KeyBind previousSkinKey{ KeyBind(KeyBind::PAGE_DOWN) };
	bool rainbowText{ false };
	bool heroName{ true };
	bool quickSkinChange{ false };
	bool drawPlayerSpells{ false };
	bool drawAllySpells{ false };
	bool drawEnemySpells{ true };
	bool drawSpellLevel{ true };
	std::int32_t current_combo_skin_index{ 0 };
	std::int32_t current_combo_ward_index{ 0 };
	std::int32_t current_combo_minion_index{ 0 };
	std::int32_t current_minion_skin_index{ -1 };
	std::int32_t current_ward_skin_index{ -1 };
	std::map<std::uint32_t, std::int32_t> current_combo_ally_skin_index;
	std::map<std::uint32_t, std::int32_t> current_combo_enemy_skin_index;
	std::map<std::uint32_t, std::int32_t> current_combo_jungle_mob_skin_index;
private:
	std::filesystem::path path;
	json config_json{ json() };
};
