#pragma once

#include <cstdint>

#include "CharacterDataStack.hpp"
#include "GameObject.hpp"

class AIBaseCommon : public GameObject {
public:
	CharacterDataStack* get_character_data_stack() noexcept { return reinterpret_cast<CharacterDataStack*>(std::uintptr_t(this) + offsets::AIBaseCommon::CharacterDataStack); }

	bool skin_model_push(const char* model, std::int32_t skin) noexcept;
	void change_skin(const char* model, std::int32_t skin) noexcept;
};
