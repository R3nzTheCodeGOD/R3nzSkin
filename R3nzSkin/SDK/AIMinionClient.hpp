#pragma once

#include <Windows.h>

#include "AIBaseCommon.hpp"

class AIMinionClient : public AIBaseCommon {
public:
	[[nodiscard]] AIBaseCommon* get_gold_redirect_target() noexcept
	{
		static const auto GetGoldRedirectTarget{ reinterpret_cast<AIBaseCommon*(__thiscall*)(void*)>(std::uintptr_t(::GetModuleHandle(nullptr)) + offsets::functions::GetGoldRedirectTarget) };
		return GetGoldRedirectTarget(this);
	}
};