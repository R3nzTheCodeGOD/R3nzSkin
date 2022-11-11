#include "AIMinionClient.hpp"

#include "../CheatManager.hpp"
#include "../Encryption.hpp"
#include "../RetSpoofInvoker.hpp"

AIBaseCommon* AIMinionClient::getGoldRedirectTarget() const noexcept
{
	return invoker.invokeThiscall<AIBaseCommon*>(std::uintptr_t(this), cheatManager.memory->base + offsets::functions::GetGoldRedirectTarget);
}

bool AIMinionClient::isLaneMinion() const noexcept
{
	return reinterpret_cast<xor_value<bool>*>(std::uintptr_t(this) + offsets::AIMinionClient::IsLaneMinion)->decrypt();
}
