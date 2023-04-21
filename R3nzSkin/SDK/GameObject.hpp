#pragma once

#include <cstdint>
#include <string>

#include "../RetSpoofInvoker.hpp"
#include "../offsets.hpp"
#include "Pad.hpp"

class GameObject {
public:
	CLASS_GETTER_P(std::string, get_name, offsets::GameObject::Name)
		CLASS_GETTER(std::int32_t, get_team, offsets::GameObject::Team)
		CLASS_GETTER(std::int32_t, get_health, offsets::GameObject::Health)
		CLASS_GETTER(std::int32_t, get_max_health, offsets::GameObject::MaxHealth)

		// Returns true for lane minions.
		[[nodiscard]] bool isLaneMinion() const noexcept { return invoker.invokeFastcall<bool>(std::uintptr_t(this) + offsets::GameObject::VTable::IsLaneMinion); }

	// Returns true for blue, red and crab.
	[[nodiscard]] bool isEliteMinion() const noexcept { return invoker.invokeFastcall<bool>(std::uintptr_t(this) + offsets::GameObject::VTable::IsEliteMinion); }

	// Returns true for dragon, baron, and rift.
	[[nodiscard]] bool isEpicMinion() const noexcept { return invoker.invokeFastcall<bool>(std::uintptr_t(this) + offsets::GameObject::VTable::IsEpicMinion); }

	// Returns true for minion.
	[[nodiscard]] bool isMinion() const noexcept { return invoker.invokeFastcall<bool>(std::uintptr_t(this) + offsets::GameObject::VTable::IsMinion); }

	// Returns true for objects that both teams can damage, such as jungle objects, gangplain barrels, etc.
	[[nodiscard]] bool isJungle() const noexcept { return invoker.invokeFastcall<bool>(std::uintptr_t(this) + offsets::GameObject::VTable::IsJungle); }
};
