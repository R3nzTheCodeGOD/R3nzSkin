#include <cstdint>
#include <string>

#include "AIBaseCommon.hpp"

#include "../encryption.hpp"
#include "../fnv_hash.hpp"
#include "../offsets.hpp"

#include "../CheatManager.hpp"

bool AIBaseCommon::checkSpecialSkins(const char* model, const std::int32_t skin) noexcept
{
	const auto stack{ this->get_character_data_stack() };
	const auto champ_name{ fnv::hash_runtime(stack->base_skin.model.str) };

	if (champ_name == FNV("Katarina") && (skin >= 29 && skin <= 36)) {
		stack->base_skin.gear = static_cast<std::int8_t>(0);
	} else if (champ_name == FNV("Renekton") && (skin >= 26 && skin <= 32)) {
		stack->base_skin.gear = static_cast<std::int8_t>(0);
	} else if (champ_name == FNV("MissFortune") && skin == 16) {
		stack->base_skin.gear = static_cast<std::int8_t>(0);
	} else if (champ_name == FNV("Lux") || champ_name == FNV("Sona")) {
		if ((skin == 7 && champ_name == FNV("Lux")) || (skin == 6 && champ_name == FNV("Sona"))) {
			stack->stack.clear();
			stack->push(model, skin);
			return true;
		} else stack->stack.clear();
	} else if (stack->base_skin.gear != static_cast<std::int8_t>(-1) && champ_name != FNV("Kayn")) {
		stack->base_skin.gear = static_cast<std::int8_t>(-1);
	}
	
	return false;
}

void AIBaseCommon::change_skin(const char* model, const std::int32_t skin) noexcept
{
	const auto stack{ this->get_character_data_stack() };
	reinterpret_cast<xor_value<std::int32_t>*>(std::uintptr_t(this) + offsets::AIBaseCommon::SkinId)->encrypt(skin);
	stack->base_skin.skin = skin;

	if (!this->checkSpecialSkins(model, skin))
		stack->update(true);
}
