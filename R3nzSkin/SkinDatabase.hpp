#pragma once

#include <cinttypes>
#include <map>
#include <string>
#include <utility>
#include <vector>

namespace SkinDatabase {
	class skin_info {
	public:
		std::string model_name;
		std::string skin_name;
		int32_t skin_id;
	};

	void load() noexcept;

	extern std::map<std::uint32_t, std::vector<skin_info>> champions_skins;
	extern std::vector<std::pair<std::uint32_t, std::string>> wards_skins;
};
