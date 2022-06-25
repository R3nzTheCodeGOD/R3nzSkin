#pragma once

#include <cstdint>
#include <cstdlib>

template <size_t Index, typename ReturnType, typename... Args>
ReturnType callVirtual(void* instance, Args... args) noexcept
{
	using Fn = ReturnType(__thiscall*)(void*, Args...);
	auto function = (*reinterpret_cast<Fn**>(instance))[Index];
	return function(instance, args...);
}
