#pragma once

#include <cstdint>

#include "../offsets.hpp"
#include "Pad.hpp"
#include "AString.hpp"

class GameObject {
public:
	CLASS_GETTER_P(const AString, get_name, offsets::GameObject::Name)

	// Sets the object name using the game string assignment function.
	[[nodiscard]] bool set_name(const char* name, std::uintptr_t module_base) const noexcept
	{
		if (!name || !module_base || !offsets::functions::AString__Assign)
			return false;
		using assign_t = std::uintptr_t(__fastcall*)(AString*, const char*);
		const auto assign = reinterpret_cast<assign_t>(module_base + offsets::functions::AString__Assign);
		auto* field = reinterpret_cast<AString*>(std::uintptr_t(this) + offsets::GameObject::Name);
		assign(field, name);
		return true;
	}
	CLASS_GETTER(std::int8_t, get_team, offsets::GameObject::Team)

	// Returns true for lane minions.
	[[nodiscard]] bool isLaneMinion() const noexcept { return CallVirtual<offsets::GameObject::VTable::IsLaneMinion, bool>(std::uintptr_t(this)); }

	// Returns true for blue, red and crab.
	[[nodiscard]] bool isEliteMinion() const noexcept { return CallVirtual<offsets::GameObject::VTable::IsEliteMinion, bool>(std::uintptr_t(this)); }

	// Returns true for dragon, baron, and rift.
	[[nodiscard]] bool isEpicMinion() const noexcept { return CallVirtual<offsets::GameObject::VTable::IsEpicMinion, bool>(std::uintptr_t(this)); }

	// Returns true for minion.
	[[nodiscard]] bool isMinion() const noexcept { return CallVirtual<offsets::GameObject::VTable::IsMinion, bool>(std::uintptr_t(this)); }

	// Returns true for objects that both teams can damage, such as jungle objects, gangplain barrels, etc.
	[[nodiscard]] bool isJungle() const noexcept { return CallVirtual<offsets::GameObject::VTable::IsJungle, bool>(std::uintptr_t(this)); }
};
