#include <cstdint>

#include "AIBaseCommon.hpp"

#include "../encryption.hpp"
#include "../fnv_hash.hpp"
#include "../offsets.hpp"

bool AIBaseCommon::skin_model_push(const char* model, std::int32_t skin) noexcept
{
	const auto champ_name{ fnv::hash_runtime(this->get_character_data_stack()->base_skin.model.str) };
	if (champ_name == FNV("Lux")) {
		if (skin == 7 && champ_name == FNV("Lux")) {
			this->get_character_data_stack()->stack.clear();
			this->get_character_data_stack()->push(model, skin);
			return true;
		}
		else
			this->get_character_data_stack()->stack.clear();
	}
	return false;
}

void AIBaseCommon::change_skin(const char* model, std::int32_t skin) noexcept
{
	reinterpret_cast<xor_value<std::int32_t>*>(std::uintptr_t(this) + offsets::AIBaseCommon::SkinId)->encrypt(skin);
	this->get_character_data_stack()->base_skin.skin = skin;

	if (!this->skin_model_push(model, skin))
		this->get_character_data_stack()->update(true);
}
