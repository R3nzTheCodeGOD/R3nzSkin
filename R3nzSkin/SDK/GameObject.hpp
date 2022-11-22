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

	// Returns true for lane minions.
	[[nodiscard]] bool isLaneMinion() const noexcept { return invoker.invokeThiscall<bool, offsets::GameObject::VTable::IsLaneMinion>(std::uintptr_t(this)); }
	
	// Returns true for blue, red and crab.
	[[nodiscard]] bool isEliteMinion() const noexcept { return invoker.invokeThiscall<bool, offsets::GameObject::VTable::IsEliteMinion>(std::uintptr_t(this)); }
	
	// Returns true for dragon, baron, and rift.
	[[nodiscard]] bool isEpicMinion() const noexcept { return invoker.invokeThiscall<bool, offsets::GameObject::VTable::IsEpicMinion>(std::uintptr_t(this)); }
	
	// Returns true for minion.
	[[nodiscard]] bool isMinion() const noexcept { return invoker.invokeThiscall<bool, offsets::GameObject::VTable::IsMinion>(std::uintptr_t(this)); }
	
	// Returns true for objects that both teams can damage, such as jungle objects, gangplain barrels, etc.
	[[nodiscard]] bool isJungle() const noexcept { return invoker.invokeThiscall<bool, offsets::GameObject::VTable::IsJungle>(std::uintptr_t(this)); }
};
