#include "AIMinionClient.hpp"

#include "../CheatManager.hpp"
#include "../Encryption.hpp"

AIBaseCommon* AIMinionClient::get_gold_redirect_target() noexcept
{
	using FnGetGoldRedirectTarget = AIBaseCommon*(__thiscall*)(void*);
	static const auto GetGoldRedirectTarget{ reinterpret_cast<FnGetGoldRedirectTarget>(cheatManager.memory->getLeagueModule() + offsets::functions::GetGoldRedirectTarget) };
	return GetGoldRedirectTarget(this);
}

bool AIMinionClient::is_lane_minion() noexcept
{
	return reinterpret_cast<xor_value<bool>*>(std::uintptr_t(this) + offsets::AIMinionClient::IsLaneMinion)->decrypt();
}
