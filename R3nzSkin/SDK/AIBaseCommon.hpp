#pragma once

#include <cstdint>

#include "CharacterDataStack.hpp"
#include "GameObject.hpp"

class AIBaseCommon : public GameObject {
public:
	[[nodiscard]] CharacterDataStack* get_character_data_stack() const noexcept { return reinterpret_cast<CharacterDataStack*>(std::uintptr_t(this) + offsets::AIBaseCommon::CharacterDataStack); }

	void change_skin(const char* model, const std::int32_t skin) noexcept;
private:
	bool checkSpecialSkins(const char* model, const std::int32_t skin) noexcept;
};
