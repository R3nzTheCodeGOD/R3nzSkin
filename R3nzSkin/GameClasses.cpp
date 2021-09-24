#include <Windows.h>

#include "encryption.hpp"
#include "fnv_hash.hpp"

#include "GameClasses.hpp"

void CharacterDataStack::push(const char* model, std::int32_t skin) noexcept
{
	static const auto Push{ reinterpret_cast<int(__thiscall*)(void*, const char* model, std::int32_t skinid, std::int32_t, bool update_spells, bool dont_update_hud, bool, bool, bool change_particle, bool, char, const char*, std::int32_t, const char*, std::int32_t, bool, std::int32_t)>(std::uintptr_t(::GetModuleHandleA(nullptr)) + offsets::functions::CharacterDataStack__Push) };
	Push(this, model, skin, 0, false, false, false, false, true, false, -1, "\x00", 0, "\x00", 0, false, 1);
}

void CharacterDataStack::update(bool change) noexcept
{
	static const auto Update{ reinterpret_cast<void(__thiscall*)(void*, bool)>(std::uintptr_t(::GetModuleHandleA(nullptr)) + offsets::functions::CharacterDataStack__Update) };
	Update(this, change);
}

bool AIBaseCommon::skin_model_push(const char* model, std::int32_t skin) noexcept
{
	auto champ_name{ fnv::hash_runtime(this->get_character_data_stack()->base_skin.model.str) };
	if (champ_name == FNV("Lux")) {
		if (skin == 7 && champ_name == FNV("Lux")) {
			this->get_character_data_stack()->stack.clear();
			this->get_character_data_stack()->push(model, skin);
			return true;
		} else
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

AIBaseCommon* AIMinionClient::get_gold_redirect_target() noexcept
{
	static const auto GetGoldRedirectTarget{ reinterpret_cast<AIBaseCommon*(__thiscall*)(void*)>(std::uintptr_t(::GetModuleHandleA(nullptr)) + offsets::functions::GetGoldRedirectTarget) };
	return GetGoldRedirectTarget(this);
}
