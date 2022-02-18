#pragma once

#include <cstdint>
#include <map>

#include "depend\json\single_include\nlohmann\json.hpp"


#include "Utils.hpp"

using json = nlohmann::json;

namespace Config {
	struct cfg {
		KeyBind menuKey{ KeyBind(KeyBind::INSERT) };
		KeyBind nextSkinKey{ KeyBind(KeyBind::PAGE_UP) };
		KeyBind previousSkinKey{ KeyBind(KeyBind::PAGE_DOWN) };
		bool rainbowText{ false };
		bool heroName{ true };
		bool quickSkinChange{ false };
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
