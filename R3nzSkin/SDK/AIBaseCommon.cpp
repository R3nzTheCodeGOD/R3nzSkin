#include <cstdint>
#include <string>

#include "AIBaseCommon.hpp"

#include "../encryption.hpp"
#include "../fnv_hash.hpp"
#include "../offsets.hpp"

#include "../CheatManager.hpp"

bool AIBaseCommon::skin_model_push(const char* model, std::int32_t skin) noexcept
{
	const auto stack{ this->get_character_data_stack() };
	const auto champ_name{ fnv::hash_runtime(this->get_character_data_stack()->base_skin.model.str) };
	if (champ_name == FNV("Lux") || champ_name == FNV("Sona")) {
		if ((skin == 7 && champ_name == FNV("Lux")) || (skin == 6 && champ_name == FNV("Sona"))) {
			stack->stack.clear();
			stack->push(model, skin);
			return true;
		} else stack->stack.clear();
	} return false;
}

void AIBaseCommon::change_skin(const char* model, std::int32_t skin) noexcept
{
	const auto stack{ this->get_character_data_stack() };
	reinterpret_cast<xor_value<std::int32_t>*>(std::uintptr_t(this) + offsets::AIBaseCommon::SkinId)->encrypt(skin);
	stack->base_skin.skin = skin;

	if (!this->skin_model_push(model, skin))
		stack->update(true);
}
