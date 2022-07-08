#pragma once

#include <cstdint>
#include <vector>

#include "CharacterStackData.hpp"

class CharacterDataStack {
public:
	std::vector<CharacterStackData> stack;
	CharacterStackData base_skin;

	void update(const bool change) noexcept;
	void push(const char* model, const std::int32_t skin) noexcept;
};
