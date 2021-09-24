#pragma once

#include <cstdint>
#include <map>

#include "json/json.hpp"

using json = nlohmann::json;

namespace Config {
	struct cfg {
		bool rainbowText{ false };
		bool heroName{ true };
		std::int32_t current_combo_skin_index{ 0 };
		std::int32_t current_combo_ward_index{ 0 };
		std::int32_t current_ward_skin_index{ -1 };
		std::map<std::uint32_t, std::int32_t> current_combo_ally_skin_index;
		std::map<std::uint32_t, std::int32_t> current_combo_enemy_skin_index;
	};

	inline auto config_json{ json() };
	inline cfg config;
	
	void save() noexcept;
	void load() noexcept;
	void reset() noexcept;
};
