#include "AIMinionClient.hpp"

#include <cstdint>

#include "../CheatManager.hpp"

AIBaseCommon* AIMinionClient::getGoldRedirectTarget() const noexcept
{
	static const auto getOwner{ reinterpret_cast<AIBaseCommon*(__fastcall*)(std::uintptr_t)>(cheatManager.memory->base + offsets::functions::GetGoldRedirectTarget) };
	return getOwner(std::uintptr_t(this));
}
