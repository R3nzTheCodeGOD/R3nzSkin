#pragma once

#include <Windows.h>
#include <cstdint>

#include "../offsets.hpp"

class Chat {
public:
	void printChat(const char* message) noexcept
	{
		static const auto PrintChat{ reinterpret_cast<void(__thiscall*)(void*, const char*, int)>(std::uintptr_t(::GetModuleHandle(nullptr)) + offsets::functions::PrintChat) };
		PrintChat(this, message, 0xFFFFFF);
	}
};