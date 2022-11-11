#include <cstdint>

#include "../CheatManager.hpp"
#include "../Offsets.hpp"
#include "../RetSpoofInvoker.hpp"

#include "CharacterDataStack.hpp"

void CharacterDataStack::push(const char* model, const std::int32_t skin) const noexcept
{
	invoker.invokeThiscall<int>(std::uintptr_t(this), cheatManager.memory->base + offsets::functions::CharacterDataStack__Push, model, skin, 0, false, false, false, false, true, false, std::int8_t(-1), "\x00", 0, "\x00", 0, false, 1);
}

void CharacterDataStack::update(const bool change) const noexcept
{
	invoker.invokeThiscall<void>(std::uintptr_t(this), cheatManager.memory->base + offsets::functions::CharacterDataStack__Update, change);
}
