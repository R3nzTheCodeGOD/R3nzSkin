#pragma once

#include <cstdint>
#include <string>

#include "../offsets.hpp"
#include "Pad.hpp"

class GameObject {
public:
	CLASS_GETTER_P(std::string, get_name, offsets::GameObject::Name)
	CLASS_GETTER(std::int32_t, get_team, offsets::GameObject::Team)

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
