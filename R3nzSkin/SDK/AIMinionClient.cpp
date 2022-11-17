#include "AIMinionClient.hpp"

#include <cstdint>

#include "../CheatManager.hpp"
#include "../RetSpoofInvoker.hpp"

AIBaseCommon* AIMinionClient::getGoldRedirectTarget() const noexcept
{
	return invoker.invokeThiscall<AIBaseCommon*>(std::uintptr_t(this), cheatManager.memory->base + offsets::functions::GetGoldRedirectTarget);
}
