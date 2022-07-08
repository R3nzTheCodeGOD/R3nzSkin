#include <cstdint>

#include "../CheatManager.hpp"
#include "../Offsets.hpp"
#include "CharacterDataStack.hpp"

void CharacterDataStack::push(const char* model, const std::int32_t skin) noexcept
{
	using FnPush = int(__thiscall*)(void*, const char* model, std::int32_t skinid, std::int32_t, bool update_spells, bool dont_update_hud, bool, bool, bool change_particle, bool, char, const char*, std::int32_t, const char*, std::int32_t, bool, std::int32_t);
	static const auto Push{ reinterpret_cast<FnPush>(cheatManager.memory->getLeagueModule() + offsets::functions::CharacterDataStack__Push) };
	Push(this, model, skin, 0, false, false, false, false, true, false, -1, "\x00", 0, "\x00", 0, false, 1);
}

void CharacterDataStack::update(const bool change) noexcept
{
	using FnUpdate = void(__thiscall*)(void*, bool);
	static const auto Update{ reinterpret_cast<FnUpdate>(cheatManager.memory->getLeagueModule() + offsets::functions::CharacterDataStack__Update) };
	Update(this, change);
}
