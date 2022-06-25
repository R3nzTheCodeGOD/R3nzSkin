#pragma once

#include <cstdint>

#include "../Offsets.hpp"

#include "CharacterDataStack.hpp"
#include "GameObject.hpp"
#include "Spell.hpp"
#include "SpellSlot.hpp"

class AIBaseCommon : public GameObject {
public:
	[[nodiscard]] CharacterDataStack* get_character_data_stack() noexcept { return reinterpret_cast<CharacterDataStack*>(std::uintptr_t(this) + offsets::AIBaseCommon::CharacterDataStack); }
	[[nodiscard]] SpellSlot* getSpellSlot(Spell slot) noexcept { return *reinterpret_cast<SpellSlot**>(std::uintptr_t(this) + offsets::SpellBook::SpellBook + offsets::SpellBook::SpellSlot + (0x4 * static_cast<int>(slot))); }

	void change_skin(const char* model, std::int32_t skin) noexcept;
private:
	bool skin_model_push(const char* model, std::int32_t skin) noexcept;
};
