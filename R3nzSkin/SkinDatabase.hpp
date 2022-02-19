#pragma once

#include <cstdint>
#include <map>
#include <string>
#include <utility>
#include <vector>

class SkinDatabase {
public:
	class skin_info {
	public:
		std::string model_name;
		std::string skin_name;
		int32_t skin_id;
	};

	void load() noexcept;

	std::map<std::uint32_t, std::vector<skin_info>> champions_skins;
	std::vector<std::pair<std::uint32_t, std::string>> wards_skins;
};
