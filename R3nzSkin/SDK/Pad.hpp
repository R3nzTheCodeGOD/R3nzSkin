#pragma once

#include <cstddef>

#define CONCAT(a, b) a##b
#define PAD_NAME(n) CONCAT(pad, n)
#define PAD(size) \
private: \
    std::byte PAD_NAME(__LINE__)[size]; \
public:

#define CLASS_GETTER(returnType, name, offset) \
[[nodiscard]] inline returnType name() const noexcept \
{ \
	return *reinterpret_cast<returnType*>(std::uintptr_t(this) + offset); \
}

#define CLASS_GETTER_P(returnType, name, offset) \
[[nodiscard]] inline returnType* name() const noexcept \
{ \
	return reinterpret_cast<returnType*>(std::uintptr_t(this) + offset); \
}

template <std::size_t Index, typename ReturnType, typename... Args>
ReturnType CallVirtual(std::uintptr_t instance, Args... args)
{
	using Fn = ReturnType(__fastcall*)(std::uintptr_t, Args...);
	const auto function{ (*reinterpret_cast<Fn**>(instance))[Index] };
	return function(instance, args...);
}