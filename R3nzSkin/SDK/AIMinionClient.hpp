#pragma once

#include <Windows.h>

#include "AIBaseCommon.hpp"
#include "../encryption.hpp"


class AIMinionClient : public AIBaseCommon {
public:
	[[nodiscard]] AIBaseCommon* get_gold_redirect_target() noexcept
	{
		static const auto GetGoldRedirectTarget{ reinterpret_cast<AIBaseCommon*(__thiscall*)(void*)>(std::uintptr_t(::GetModuleHandle(nullptr)) + offsets::functions::GetGoldRedirectTarget) };
		return GetGoldRedirectTarget(this);
	}

	[[nodiscard]] bool is_lane_minion() noexcept { return reinterpret_cast<xor_value<bool>*>(std::uintptr_t(this) + offsets::AIMinionClient::IsLaneMinion)->decrypt(); }
};